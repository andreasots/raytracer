#include <iostream>
#include <fstream>
#include <cassert>

#include <gmtl/Ray.h>
#include "raytracer/color.h"
#include "raytracer/scene.h"
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>
#include <gmtl/Output.h>

#include <assimp/aiDefines.h>
#include <assimp/aiVersion.h>

#include <cmath>

#include "version.h"
#include <gmtl/Version.h>
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
    std::cout << "\tGMTL: " << gmtl::getVersion() << std::endl;
    std::cout << "\tPixelToaster: " << PIXELTOASTER_VERSION << std::endl;
    std::cout << "\tOpenEXR" << std::endl;
    std::cout << std::endl;

    w = 800;
    h = 600;

    PixelToaster::Display screen("Raytracer", w, h);
    std::vector<PixelToaster::Pixel> fb(w*h); // Front buffer
    std::vector<Raytracer::Color<>> bb(w*h); // Back buffer

    RT_FLOAT x = -DISTANCE * 18 / FOCAL_LENGTH;
    RT_FLOAT y = -x*h/w;
    RT_FLOAT z = -DISTANCE;

    gmtl::Point<RT_FLOAT, 3> scr_pos(x, y, z);
    gmtl::Point<RT_FLOAT, 3> scr_x(-x, y, z);
    gmtl::Point<RT_FLOAT, 3> scr_y(x, -y, z);
    gmtl::Point<RT_FLOAT, 3> pos(0,0,0);

	try
	{
        std::ifstream cam("camera.xform");
        cam.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
        gmtl::Matrix<RT_FLOAT, 4, 4> mat;
        RT_FLOAT data[16];
        for(size_t i = 0; i < 16; i++)
            cam >> data[i];
        mat.set/*Transpose*/(data);
        pos = mat * pos;
        scr_pos = mat * scr_pos;
        scr_x = mat * scr_x;
        scr_y = mat * scr_y;
	}
	catch(...)
	{
	}

	gmtl::Vec<RT_FLOAT, 3> delta_x = (scr_x-scr_pos)/static_cast<RT_FLOAT>(w);
	gmtl::Vec<RT_FLOAT, 3> delta_y = (scr_y-scr_pos)/static_cast<RT_FLOAT>(h);

    Raytracer::Scene scene;
    scene.open(argv[1]);

    PixelToaster::Timer timer, updated;
    bool render = true;
    size_t sample = 1;
    while(render)
    {
        std::cout << "Sample " << sample << std::endl;
        for(size_t y = 0; y < h && render; y++)
        {
            std::cout << "Scanline " << y+1 << '\r' << std::flush;
            #pragma omp parallel for schedule(dynamic, 1)
            for(size_t x = 0; x < w; x++)
            {
                Raytracer::Color<> pixel(bb[y*w+x]);
                pixel.mult(sample-1);

                gmtl::Point<RT_FLOAT, 3> scr = scr_pos + delta_y*static_cast<RT_FLOAT>(drand48()+y) + delta_x*static_cast<RT_FLOAT>(drand48()+x);
                gmtl::Vec<RT_FLOAT, 3> dir = scr - pos;
                gmtl::normalize(dir);

                pixel.add(scene.radiance(gmtl::Ray<RT_FLOAT>(pos, dir), 0));

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
        std::cout << std::endl;

        Imf::Rgba *pixels = new Imf::Rgba[w*h];
        for(size_t Y = 0; Y < h; Y++)
            for(size_t X = 0; X < w; X++)
                pixels[X+Y*w] = Imf::Rgba(bb[X+Y*w].r(), bb[X+Y*w].g(), bb[X+Y*w].b());
        Imf::RgbaOutputFile file("image.exr", w, h, Imf::WRITE_RGBA);
        file.setFrameBuffer(pixels, 1, w);
        file.writePixels(h);
        delete[] pixels;
        sample++;
    }

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
