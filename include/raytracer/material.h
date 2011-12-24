#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include <SIMD/Vec.h>
#include <SIMD/Ray.h>

namespace Raytracer
{
    class Scene;
    class Color;
    struct Material
    {
        virtual ~Material()
        {
        }
        virtual Color color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth) const = 0;
    };
}

#endif
