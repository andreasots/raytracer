#ifndef RAYTRACER_MATERIAL_NULL_H
#define RAYTRACER_MATERIAL_NULL_H

#include "raytracer/material.h"
#include "raytracer/color.h"

namespace Raytracer {
namespace material {

class Null: public Raytracer::Material
{
    public:
        Null(Color _color);
        virtual ~Null();
        Color color(const SIMD::Point &p, const SIMD::Matrix &m, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const;
    protected:
        Color m_color;
    private:
};

} // namespace Material
} // namespace Raytracer

#endif // RAYTRACER_MATERIAL_NULL_H
