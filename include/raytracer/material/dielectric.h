#ifndef RAYTRACER_MATERIAL_DIELECTRIC_H
#define RAYTRACER_MATERIAL_DIELECTRIC_H

#include "raytracer/material.h"

namespace Raytracer {
namespace material {

class Dielectric: public Raytracer::Material
{
    public:
        Dielectric();
        virtual ~Dielectric();
        Color color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth) const;
    protected:
    private:
};

} // namespace Material
} // namespace Raytracer

#endif // RAYTRACER_MATERIAL_DIELECTRIC_H
