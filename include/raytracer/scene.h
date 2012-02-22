#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "raytracer/color.h"
#include "raytracer/material.h"
#include "raytracer/object.h"
#include "raytracer/texture.h"

#include "raytracer/octree/root.h"

#include <SIMD/AABox.h>
#include <SIMD/Ray.h>
#include <SIMD/Matrix.h>

#include <dSFMT.h>

#include <vector>
#include <utility>
#include <string>

namespace Raytracer {
class Scene
{
    public:
        Scene();
        virtual ~Scene();
        SIMD::Matrix open(std::string file);
        void add(Object *o);
        RT_FLOAT intersect(const SIMD::Ray &r, size_t &id, RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max = HUGE_VAL);
        Color radiance(const SIMD::Ray &r, size_t depth, dsfmt_t &dsfmt);
        void stats();
    protected:
    private:
        Octree::Root octree;
        std::vector<Object *> m_objects;
        unsigned long long m_intersections, m_hits;
        enum {SKY_TEX, SKY_COL} m_sky_type;
        Color m_sky_col;
        Texture m_sky_tex;
};

} // namespace Raytracer

#endif // RAYTRACER_SCENE_H
