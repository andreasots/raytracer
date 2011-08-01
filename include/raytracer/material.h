#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "raytracer/color.h"

namespace Raytracer
{
    struct Material
    {
        FLOAT reflect, specular, spec_pow;
        Color<> color;
        bool emit;

        Material(): reflect(0), specular(0.5), spec_pow(20), color(), emit(false)
        { }
    };
}

#endif
