#include <iostream>
#include <fstream>

#include <gmtl/Ray.h>
#include "raytracer/color.h"
#include "raytracer/scene.h"
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

#include <cmath>

#include "version.h"
#include <gmtl/Version.h>
#define SAMPLES 4
size_t w, h;

int main(int argc, char *argv[])
{
    std::cout << "Raytracer " << AutoVersion::FULLVERSION_STRING << " build ";
    std::cout << AutoVersion::BUILD << " (" << AutoVersion::STATUS << ")";
    std::endl(std::cout);

    std::cout << "Libraries: " << std::endl;
    std::cout << "\tGMTL: " << gmtl::getVersion() << std::endl;

    w = 512;
    h = 512;

    FLOAT m_SX, m_SY;
    // screen plane in world space coordinates
	FLOAT m_WX1 = -4, m_WX2 = 4, m_WY1 = 4, m_WY2 =  m_SY = -4;
	// calculate deltas for interpolation
	FLOAT m_DX = (m_WX2 - m_WX1) / w;
	FLOAT m_DY = (m_WY2 - m_WY1) / h;

    Raytracer::Scene scene("");

    std::ofstream out("image.ppm");

    out << "P3" << std::endl << w << std::endl << h << std::endl << 255;
    out << std::endl;

    gmtl::Point<FLOAT, 3> pos(0,0,-5);
    for(size_t y = 0; y < h; y++)
    {
        std::cout << "Scanline " << y+1 << "/" << h << "\r" << std::flush;
        for(size_t x = 0; x < w; x++)
        {
            Raytracer::Color<> pixel;
            for(size_t sample = 0; sample < SAMPLES; sample++)
            {
                m_SY = m_WY1 + m_DY*(drand48()+y);
                m_SX = m_WX1 + m_DX*(drand48()+x);

                gmtl::Point<FLOAT, 3> screen(m_SX, m_SY, 0);
                gmtl::Vec<FLOAT, 3> dir = screen - pos;
                gmtl::normalize(dir);

                gmtl::Ray<FLOAT> ray(pos, dir);
                pixel.add(scene.radiance(ray, 0));
            }
            pixel.mult(1.0/SAMPLES);
            pixel.gamma();
            pixel.clamp();

            out << static_cast<int>(pixel.r()*255) << " ";
            out << static_cast<int>(pixel.g()*255) << " ";
            out << static_cast<int>(pixel.b()*255) << " ";
        }
        out << std::endl;
    }
    std::endl(std::cout);

    scene.stats();

    return 0;
}
