#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "raytracer/ray.h"
#include "raytracer/color.h"
#include "raytracer/object.h"

#include <vector>

namespace Raytracer {

class Scene
{
    public:
        Scene(std::string file);
        void add(Object *o);
        float intersect(Ray<> r, size_t &id);
        Color<> radiance(Ray<> r, size_t depth);
        void stats();
    protected:
    private:
        std::vector<Object*> m_objects;
        unsigned long long m_intersections_total, m_intersections_successful;
};

} // namespace Raytracer

#endif // RAYTRACER_SCENE_H
