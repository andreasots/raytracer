#ifndef RAYTRACER_MATERIAL_ASHIKHMINSHIRLEY_H
#define RAYTRACER_MATERIAL_ASHIKHMINSHIRLEY_H

#include "raytracer/material.h"
#include "raytracer/color.h"

namespace Raytracer {
namespace material {

class AshikhminShirley : public Material
{
    public:
        AshikhminShirley(Color _Rd, RT_FLOAT _nu, RT_FLOAT _nv, Color _Rs);
        virtual ~AshikhminShirley();
        Color color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const;
    protected:
        Color Rd, Rs;
        RT_FLOAT nv, nu;
    private:
};

} // namespace material
} // namespace Raytracer

#endif // RAYTRACER_MATERIAL_ASHIKHMINSHIRLEY_H
