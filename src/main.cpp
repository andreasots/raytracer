#include <iostream>
#include <cassert>

#include <gmtl/Ray.h>
#include "raytracer/color.h"
#include "raytracer/scene.h"
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

#include <assimp/aiDefines.h>
#include <assimp/aiVersion.h>

#include <cmath>

#include "version.h"
#include <gmtl/Version.h>
#include <PixelToaster.h>

#undef FLOAT
#include <ImfRgbaFile.h>
#define FLOAT __FLOAT

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

    gmtl::Point<FLOAT, 3> top, bottom;
    bottom[0] = DISTANCE*18/FOCAL_LENGTH;
	top[0] = -bottom[0];
	top[1] = bottom[0]*h/w;
	bottom[1] = -top[1];
	top[2] = bottom[2] = -DISTANCE;

	gmtl::Vec<FLOAT, 3> delta_x, delta_y;
	delta_x[0] = (bottom[0] - top[0]) / w;
	delta_y[1] = (bottom[1] - top[1]) / h;

    Raytracer::Scene scene;
    scene.open(argv[1]);

    PixelToaster::Timer timer;

    gmtl::Point<FLOAT, 3> pos(0,0,0);
    bool render = true;
    size_t sample = 1;
    while(render)
    {
        std::cout << "Sample " << sample << std::endl;
        for(size_t y = 0; y < h; y++)
        {
            std::cout << "Scanline " << y+1 << '\r' << std::flush;
            #pragma omp parallel for schedule(dynamic, 1)
            for(size_t x = 0; x < w; x++)
            {
                Raytracer::Color<> pixel(bb[y*w+x]);
                pixel.mult(sample-1);

                gmtl::Point<FLOAT, 3> scr = top + delta_y*static_cast<FLOAT>(drand48()+y) + delta_x*static_cast<FLOAT>(drand48()+x);
                gmtl::Vec<FLOAT, 3> dir = scr - pos;
                gmtl::normalize(dir);

                pixel.add(scene.radiance(gmtl::Ray<FLOAT>(pos, dir), 0));

                pixel.mult(1.0/sample);

                bb[y*w+x] = pixel;
                pixel.gamma();
                fb[y*w+x] = pixel.PT();
            }
            screen.update(fb);
            if(!screen.open())
            {
                render = false;
                break;
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
