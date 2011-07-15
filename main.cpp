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

FLOAT clamp(FLOAT val)
{
    return val > 1 ? 1 : (val < 0 ? 0 : val);
}

FLOAT tan_fov_x, tan_fov_y;
size_t w, h;

void cam(FLOAT &x, FLOAT &y)
{
    x = tan_fov_x*(2*x-w)/w;
    y = tan_fov_y*(2*y-h)/h;
}

int main(int argc, char *argv[])
{
    std::cout << "Raytracer " << AutoVersion::FULLVERSION_STRING << " build ";
    std::cout << AutoVersion::BUILD << " (" << AutoVersion::STATUS << ")";
    std::endl(std::cout);

    std::cout << "Libraries: " << std::endl;
    std::cout << "\tGMTL: " << gmtl::getVersion() << std::endl;

    w = 1024;
    h = 786;

    FLOAT m_SX, m_SY;
    // screen plane in world space coordinates
	FLOAT m_WX1 = -4, m_WX2 = 4, m_WY1 = 3, m_WY2 =  m_SY = -3;
	// calculate deltas for interpolation
	FLOAT m_DX = (m_WX2 - m_WX1) / w;
	FLOAT m_DY = -(m_WY2 - m_WY1) / h;

    Raytracer::Color<> pixel;

    Raytracer::Scene scene("");

    std::ofstream out("image.ppm");
    out << "P3" << std::endl;
    out << w << std::endl;
    out << h << std::endl;
    out << 255 << std::endl;

    for(size_t y = 0; y < h; y++)
    {
        std::cout << "Scanline " << y+1 << "/" << h << "\r" << std::flush;
        m_SX = m_WX1;
        for(size_t x = 0; x < w; x++)
        {
            gmtl::Point<FLOAT, 3> screen(m_SX, m_SY, 0), pos(0,0,-5);
            gmtl::Vec<FLOAT, 3> dir = screen - pos;
            gmtl::normalize(dir);

            gmtl::Ray<FLOAT> ray(pos, dir);
            pixel = scene.radiance(ray, 0);

            out << static_cast<int>(clamp(pixel.r())*255) << " ";
            out << static_cast<int>(clamp(pixel.g())*255) << " ";
            out << static_cast<int>(clamp(pixel.b())*255) << " ";
            m_SX += m_DX;
        }
        out << std::endl;
        m_SY += m_DY;
    }
    std::endl(std::cout);
    out << std::endl;

    scene.stats();

    return 0;
}
