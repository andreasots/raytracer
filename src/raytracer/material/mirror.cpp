#include "raytracer/material/mirror.h"
#include "raytracer/scene.h"
#include "raytracer/color.h"
#include <cstdlib>

namespace Raytracer {
namespace material {

Mirror::Mirror()
{
    //ctor
}

Mirror::~Mirror()
{
    //dtor
}

Color Mirror::color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const
{
    return scene.radiance(SIMD::Ray(p, in+std::abs(2*n.dot(in))*n), depth+1, dsfmt);
}

} // namespace material
} // namespace Raytracer
