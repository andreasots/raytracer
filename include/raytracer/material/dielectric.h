#ifndef RAYTRACER_MATERIAL_DIELECTRIC_H
#define RAYTRACER_MATERIAL_DIELECTRIC_H

#include "raytracer/material.h"

namespace Raytracer {
namespace material {

class Dielectric: public Raytracer::Material
{
    public:
        Dielectric(RT_FLOAT n);
        virtual ~Dielectric();
        Color color(const SIMD::Point &p, const SIMD::Matrix &m, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const;
    protected:
        RT_FLOAT m_n;
    private:
};

} // namespace Material
} // namespace Raytracer

#endif // RAYTRACER_MATERIAL_DIELECTRIC_H
