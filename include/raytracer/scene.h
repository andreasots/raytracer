#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "raytracer/color.h"
#include "raytracer/material.h"
#include "raytracer/octree.h"

#include <gmtl/AABox.h>
#include <gmtl/Ray.h>
#include <gmtl/Sphere.h>
#include <gmtl/Plane.h>
#include <gmtl/Tri.h>

#include <vector>
#include <utility>
#include <string>

namespace Raytracer {
class Scene
{
    public:
        Scene();
        void open(std::string file);
        void add(Object *o);
        FLOAT intersect(const gmtl::Ray<FLOAT> &r, size_t &id, FLOAT max = HUGE_VAL);
        Color<> radiance(const gmtl::Ray<FLOAT> &r, size_t depth);
        void stats();
    protected:
    private:
        void regenerate();
        std::vector<Object *> m_objects;
        std::vector<size_t> m_emit;
        unsigned long long m_intersections, m_hits;
        Octree *m_octree;
};

} // namespace Raytracer

#endif // RAYTRACER_SCENE_H
