#ifndef RAYTRACER_MATERIAL_MIRROR_H
#define RAYTRACER_MATERIAL_MIRROR_H

#include "raytracer/material.h"

namespace Raytracer {
namespace material {

class Mirror : public Raytracer::Material
{
    public:
        Mirror();
        virtual ~Mirror();
        Color color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const;
    protected:
    private:
};

} // namespace material
} // namespace Raytracer

#endif // RAYTRACER_MATERIAL_MIRROR_H
