#include <iostream>
#include <fstream>

#include "raytracer/ray.h"
#include "raytracer/color.h"
#include "raytracer/scene.h"
#include "raytracer/vector.h"

#include <cmath>

#include "version.h"
float clamp(float val)
{
    return val > 1 ? 1 : (val < 0 ? 0 : val);
}

float tan_fov_x, tan_fov_y;
size_t w, h;

void cam(float &x, float &y)
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
    std::cout << "\tAllegro: >5.0.0 (no exact version information)"<<std::endl;

    w = 1024;
    h = 786;

    tan_fov_x = tan(M_PI_2);
    tan_fov_y = tan(M_PI_2*h/w);

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
        for(size_t x = 0; x < w; x++)
        {
            Raytracer::Vector<> dir, pos = {0,0,-5};
            cam(dir[0], dir[1]);
            dir[2] -= pos[2];
            dir.normalise();

            if(y == h/2 && x == w/2)
                std::cout << dir[0] << ' ' << dir[1] << ' ' << dir[2] << std::endl;

            Raytracer::Ray<> ray(pos, dir);
            pixel = scene.radiance(ray, 0);

            out << static_cast<int>(clamp(pixel.r())*255) << " ";
            out << static_cast<int>(clamp(pixel.g())*255) << " ";
            out << static_cast<int>(clamp(pixel.b())*255) << " ";
        }
        out << std::endl;
    }
    std::endl(std::cout);
    out << std::endl;

    scene.stats();

    pixel = scene.radiance(Raytracer::Ray<>({0,0,-5}, {0,0,1}),0);
    std::cout << pixel.r() << ' ' << pixel.g() << ' ' << pixel.b() << std::endl;
    return 0;
}
