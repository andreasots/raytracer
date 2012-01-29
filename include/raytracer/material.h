#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include <SIMD/Vec.h>
#include <SIMD/Ray.h>
#include <SIMD/Matrix.h>

#include <dSFMT.h>

namespace Raytracer
{
    class Scene;
    class Color;
    struct Material
    {
        virtual ~Material()
        {
        }
        virtual Color color(const SIMD::Point &p, const SIMD::Matrix &m, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const = 0;
    };
}

#endif
