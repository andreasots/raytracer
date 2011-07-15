#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "raytracer/color.h"
#include "raytracer/material.h"

#include <gmtl/Ray.h>
#include <gmtl/Sphere.h>
#include <gmtl/Plane.h>
#include <gmtl/Tri.h>

#include <vector>
#include <utility>
#include <string>

namespace Raytracer {
enum ObjectType
{
    PLANE,
    SPHERE,
    TRI
};
class Scene
{
    public:
        Scene(std::string file);
        void add(gmtl::Sphere<FLOAT> sphere, Material mat);
        void add(gmtl::Plane<FLOAT> plane, Material mat);
        void add(gmtl::Tri<FLOAT> tri, Material mat);
        FLOAT intersect(gmtl::Ray<FLOAT> r, ObjectType &type, size_t &id);
        Color<> radiance(gmtl::Ray<FLOAT> r, size_t depth);
        void stats();
    protected:
    private:
        std::vector<std::pair<gmtl::Plane<FLOAT>, Material>> m_planes;
        std::vector<std::pair<gmtl::Sphere<FLOAT>, Material>> m_spheres;
        std::vector<std::pair<gmtl::Tri<FLOAT>, Material>> m_tris;
        unsigned long long m_intersections, m_hits;
};

} // namespace Raytracer

#endif // RAYTRACER_SCENE_H
