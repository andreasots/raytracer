#include <iostream>
#include <fstream>
#include <cassert>

#include <SIMD/Point.h>
#include <SIMD/Vec.h>
#include <SIMD/Matrix.h>

#include <dSFMT.h>
#include <cstdlib>
#include <ctime>

#include "raytracer/color.h"
#include "raytracer/scene.h"

#include <cmath>

#include "version.h"
#include <PixelToaster.h>

#include <ImfRgbaFile.h>

#include <omp.h>

// Focal length in mm
#define FOCAL_LENGTH 35.0
#define DISTANCE 5.0
size_t w, h;

#define TILESIZE 32

// Cubic B-Spline Filter
// Generation of Stratified Samples for B-Spline Pixel Filtering
// Michael Stark, Peter Shirley, Michael Ashikhmin
inline RT_FLOAT cubicBSplineFilter(RT_FLOAT r)
{
    if(r < 1.0/24)
        return std::pow(24*r, 0.25)-2;
    else if(r < 0.5)
    {
        r = 24*r-1;
        RT_FLOAT u = r/11;
        for(int i = 0; i < 5; i++)
            u = (r + u*u*(6+u*(8-9*u)))/(4+12*u*(1+u*(1-u)));
        return u - 1;
    }
    else if(r < 23.0/24)
    {
        r = 23-24*r;
        RT_FLOAT u = r/11;
        for(int i = 0; i < 5; i++)
            u = (r + u*u*(6+u*(8-9*u)))/(4+12*u*(1+u*(1-u)));
        return 1-u;
    }
    else
        return 2-std::pow(24-24*r, 0.25);
}

int main(int argc, char *argv[])
{
    std::cout << "Raytracer " << AutoVersion::FULLVERSION_STRING << " build ";
    std::cout << AutoVersion::BUILD << " (" << AutoVersion::STATUS << ")";
    std::endl(std::cout);
    std::cout << std::endl;

    std::cout << "Libraries: " << std::endl;
    std::cout << "\tPixelToaster: " << PIXELTOASTER_VERSION << std::endl;
    std::cout << "\tOpenEXR" << std::endl;
    std::cout << std::endl;

    w = 1024*3/4;
    h = 1024*3/4;

    if((w / TILESIZE)*TILESIZE != w)
        w = (w/TILESIZE+1)*TILESIZE;
    if((h / TILESIZE)*TILESIZE != h)
        h = (h/TILESIZE+1)*TILESIZE;
    std::cout << "Image: " << w << "x" << h << std::endl;

    PixelToaster::Display screen("Raytracer", w, h);
    std::vector<PixelToaster::Pixel> fb(w*h); // Front buffer
    std::vector<Raytracer::Color> bb(w*h); // Back buffer

    RT_FLOAT x = -DISTANCE * 18 / FOCAL_LENGTH;
    RT_FLOAT y = -x*h/w;
    RT_FLOAT z = -DISTANCE;

    SIMD::Point scr_pos(x, y, z);
    SIMD::Point scr_x(-x, y, z);
    SIMD::Point scr_y(x, -y, z);
    SIMD::Point pos(0,0,0);


    Raytracer::Scene scene;
    SIMD::Matrix mat = scene.open(argv[1]);;
    pos = mat * pos;
    pos.normalize();
    scr_pos = mat * scr_pos;
    scr_pos.normalize();
    scr_x = mat * scr_x;
    scr_x.normalize();
    scr_y = mat * scr_y;
    scr_y.normalize();

	SIMD::Vec delta_x = (scr_x-scr_pos)/static_cast<RT_FLOAT>(w);
	SIMD::Vec delta_y = (scr_y-scr_pos)/static_cast<RT_FLOAT>(h);
    PixelToaster::Timer timer, updated;
    bool render = true;
    size_t sample = 1;
    dsfmt_t dSFMT_states[omp_get_max_threads()];
    std::srand(time(NULL));
    for(int i = 0; i < omp_get_max_threads(); i++)
        dsfmt_chk_init_gen_rand(&dSFMT_states[i], std::rand(), DSFMT_MEXP);

    while(render)
    {
        for(size_t y = 0; y < h; y += TILESIZE)
        {
            #pragma omp parallel for schedule(dynamic, 1)
            for(size_t x = 0; x < w; x += TILESIZE)
            {
                for(size_t tile_y = 0; tile_y < TILESIZE; tile_y++)
                    for(size_t tile_x = 0; tile_x < TILESIZE; tile_x++)
                    {
                        Raytracer::Color pixel(bb[(y+tile_y)*w+x+tile_x]);
                        pixel.mult(sample-1);

                        RT_FLOAT sample_x = cubicBSplineFilter(dsfmt_genrand_close_open(&dSFMT_states[omp_get_thread_num()]));
                        RT_FLOAT sample_y = cubicBSplineFilter(dsfmt_genrand_close_open(&dSFMT_states[omp_get_thread_num()]));

                        SIMD::Point scr = scr_pos + delta_y*static_cast<RT_FLOAT>(y+tile_y+sample_y) + delta_x*static_cast<RT_FLOAT>(x+tile_x+sample_x);
                        SIMD::Vec dir = scr - pos;
                        dir.normalize();

                        pixel.add(scene.radiance(SIMD::Ray(pos, dir), 0, dSFMT_states[omp_get_thread_num()]));
                        pixel.div(sample);

                        bb[(y+tile_y)*w+x+tile_x] = pixel;
                        pixel.gamma();
                        fb[(y+tile_y)*w+x+tile_x] = pixel.PT();
                    }

                #pragma omp critical
                if(updated.time() > 1)
                {
                    std::cout << "\r\033[K[" << timer.time() <<  "] Sample " << sample << ": " << 100.0*y/h << " %" << std::flush;
                    screen.update(fb);
                    updated.reset();
                }
            }
        }
        if(!screen.open())
            break;
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

    std::cout << std::endl;
    scene.stats();
    std::cout << "Number of samples per pixel: " << sample << std::endl;
    std::cout << "Time: " << timer.time() << std::endl;

    while(screen.open())
    {
        timer.wait(0.4);
        screen.update(fb);
    }

    return 0;
}
