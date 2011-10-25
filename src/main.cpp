#include <iostream>
#include <fstream>
#include <cassert>

#include <SIMD/Point.h>
#include <SIMD/Vec.h>
#include <SIMD/Matrix.h>

#include "raytracer/color.h"
#include "raytracer/scene.h"
#include "raytracer/sphere.h"

#include <assimp/aiDefines.h>
#include <assimp/aiVersion.h>

#include <cmath>

#include "version.h"
#include <PixelToaster.h>

#include <ImfRgbaFile.h>

// Focal length in mm
#define FOCAL_LENGTH 35.0
#define DISTANCE 5.0
size_t w, h;

int main(int argc, char *argv[])
{
    std::cout << "Raytracer " << AutoVersion::FULLVERSION_STRING << " build ";
    std::cout << AutoVersion::BUILD << " (" << AutoVersion::STATUS << ")";
    std::endl(std::cout);
    std::cout << std::endl;

    std::cout << "Libraries: " << std::endl;
    std::cout << "\tAssimp: " << aiGetVersionMajor()<<'.'<<aiGetVersionMinor();
    std::cout << '.' << aiGetVersionRevision() << std::endl;
    std::cout << "\tPixelToaster: " << PIXELTOASTER_VERSION << std::endl;
    std::cout << "\tOpenEXR" << std::endl;
    std::cout << std::endl;

    w = 1024;
    h = 768;

    std::cout << SIMD::Vec(1, 0, 0).cross(SIMD::Vec(0, 1, 0)) << std::endl;

    PixelToaster::Display screen("Raytracer", w, h);
    std::vector<PixelToaster::Pixel> fb(w*h); // Front buffer
    std::vector<Raytracer::Color<>> bb(w*h); // Back buffer

    RT_FLOAT x = -DISTANCE * 18 / FOCAL_LENGTH;
    RT_FLOAT y = -x*h/w;
    RT_FLOAT z = -DISTANCE;

    SIMD::Point scr_pos(x, y, z);
    SIMD::Point scr_x(-x, y, z);
    SIMD::Point scr_y(x, -y, z);
    SIMD::Point pos(0,0,0);

	try
	{
        std::ifstream cam("camera.xform");
        cam.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);

        RT_FLOAT data[16];
        for(size_t i = 0; i < 16; i++)
            cam >> data[i];
        SIMD::Matrix mat(data);
        pos = mat * pos;
        scr_pos = mat * scr_pos;
        scr_x = mat * scr_x;
        scr_y = mat * scr_y;
	}
	catch(...)
	{
	}

	SIMD::Vec delta_x = (scr_x-scr_pos)/static_cast<RT_FLOAT>(w);
	SIMD::Vec delta_y = (scr_y-scr_pos)/static_cast<RT_FLOAT>(h);

    Raytracer::Scene scene;
    scene.open(argv[1]);

    PixelToaster::Timer timer, updated;
    bool render = true;
    size_t sample = 1;
    while(render)
    {
        std::cout << "\r\033[K[" << timer.time() <<  "] Sample " << sample << std::endl;
        for(size_t y = 0; y < h && render; y++)
        {
            #pragma omp parallel for schedule(dynamic, 1)
            for(size_t x = 0; x < w; x++)
            {
                Raytracer::Color<> pixel(bb[y*w+x]);
                pixel.mult(sample-1);

                SIMD::Point scr = scr_pos + delta_y*static_cast<RT_FLOAT>(drand48()+y) + delta_x*static_cast<RT_FLOAT>(drand48()+x);
                SIMD::Vec dir = scr - pos;
                dir.normalize();

                pixel.add(scene.radiance(SIMD::Ray(pos, dir), 0));

                pixel.div(sample);

                bb[y*w+x] = pixel;
                pixel.gamma();
                fb[y*w+x] = pixel.PT();
                #pragma omp critical
                if(updated.time() > 1)
                {
                    screen.update(fb);
                    updated.reset();
                }
                if(!screen.open())
                    render = false;
            }
        }
        sample++;
    }

    Imf::Rgba *pixels = new Imf::Rgba[w*h];
    for(size_t Y = 0; Y < h; Y++)
        for(size_t X = 0; X < w; X++)
            pixels[X+Y*w] = Imf::Rgba(bb[X+Y*w].r(), bb[X+Y*w].g(), bb[X+Y*w].b());
    Imf::RgbaOutputFile file("image.exr", w, h, Imf::WRITE_RGBA);
    file.setFrameBuffer(pixels, 1, w);
    file.writePixels(h);
    delete[] pixels;

    scene.stats();
    std::cout << "Number of samples per pixel: " << sample-1 << std::endl;
    std::cout << "Time: " << timer.time() << std::endl;

    while(screen.open())
    {
        timer.wait(0.4);
        screen.update(fb);
    }

    return 0;
}
