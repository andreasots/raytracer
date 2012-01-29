#include "raytracer/material/null.h"
#include "raytracer/color.h"
#include "raytracer/scene.h"

namespace Raytracer {
namespace material {

Null::Null(Color _color): m_color(_color)
{
    //ctor
}

Null::~Null()
{
    //dtor
}

Color Null::color(const SIMD::Point &p, const SIMD::Matrix &m, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const
{
    return m_color;
}

} // namespace Material
} // namespace Raytracer
