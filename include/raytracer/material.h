#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "raytracer/color.h"

namespace Raytracer
{
    struct Material
    {
        float reflect;
        Color<> diffuse;
        Color<> emit;
    };
}

#endif
