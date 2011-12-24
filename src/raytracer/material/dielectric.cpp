#include "raytracer/material/dielectric.h"
#include "raytracer/color.h"
namespace Raytracer {
namespace material {

Dielectric::Dielectric()
{
    //ctor
}

Dielectric::~Dielectric()
{
    //dtor
}

Color Dielectric::color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth) const
{
    return Color();
}

} // namespace Material
} // namespace Raytracer
