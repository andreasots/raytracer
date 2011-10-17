#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "raytracer/color.h"
#include "raytracer/material.h"
#include "raytracer/octree.h"

#include <SIMD/AABox.h>
#include <SIMD/Ray.h>

#include <vector>
#include <utility>
#include <string>

namespace Raytracer {
class Scene
{
    public:
        Scene();
        virtual ~Scene();
        void open(std::string file);
        void add(Object *o);
        RT_FLOAT intersect(const SIMD::Ray &r, size_t &id, RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max = HUGE_VAL);
        Color<> radiance(const SIMD::Ray &r, size_t depth);
        void stats();
    protected:
    private:
        void regenerate();
        std::vector<Object *> m_objects;
        unsigned long long m_intersections, m_hits;
        Octree *m_octree;
};

} // namespace Raytracer

#endif // RAYTRACER_SCENE_H
