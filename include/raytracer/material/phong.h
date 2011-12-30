#ifndef RAYTRACER_MATERIAL_PHONG_H
#define RAYTRACER_MATERIAL_PHONG_H

#include "raytracer/material.h"
#include "raytracer/color.h"

namespace Raytracer {
namespace material {

class Phong: public Raytracer::Material
{
    public:
        Phong(RT_FLOAT _diffuse, Color c_diff, RT_FLOAT _specular, RT_FLOAT _spec_pow, Color c_spec, RT_FLOAT _emit, Color c_e);
        virtual ~Phong();
        Color color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const;
    protected:
        RT_FLOAT diffuse, specular, spec_pow;
        Color c_diffuse, c_specular, c_emit;
    private:
};

} // namespace Material
} // namespace Raytracer

#endif // RAYTRACER_MATERIAL_LAMBERTPHONG_H
