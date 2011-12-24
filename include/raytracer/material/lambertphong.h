#ifndef RAYTRACER_MATERIAL_LAMBERTPHONG_H
#define RAYTRACER_MATERIAL_LAMBERTPHONG_H

#include "raytracer/material.h"
#include "raytracer/color.h"

namespace Raytracer {
namespace material {

class LambertPhong: public Raytracer::Material
{
    public:
        LambertPhong(RT_FLOAT _diffuse, RT_FLOAT _specular, RT_FLOAT _spec_pow, bool _emit, Color _color);
        virtual ~LambertPhong();
        Color color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth) const;
    protected:
        RT_FLOAT diffuse, specular, spec_pow;
        Color m_color;
        bool emit;
    private:
};

} // namespace Material
} // namespace Raytracer

#endif // RAYTRACER_MATERIAL_LAMBERTPHONG_H
