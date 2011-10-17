#ifndef RAYTRACER_OCTREE_H
#define RAYTRACER_OCTREE_H

#include <vector>
#include <SIMD/AABox.h>
#include <SIMD/Ray.h>
#include <SIMD/Point.h>

#include "raytracer/object.h"

namespace Raytracer {

class Octree: public SIMD::AABox
{
    public:
        Octree(const SIMD::Point &min,
               const SIMD::Point &max, Octree *parent);
        virtual ~Octree();
        bool add(Object *o, const size_t &id);
        bool intersect(const SIMD::Ray &r, RT_FLOAT &ret, size_t &id, RT_FLOAT &u, RT_FLOAT &v,
                       const std::vector<Object *> &objects,
                       unsigned long long &hits, unsigned long long &intersections);
        void prune(const std::vector<Object *> &objects);
        void createSubnodes();
        bool empty;
    protected:
        std::vector<size_t> m_objects;
        Octree *m_subnodes[8];
        Octree *m_parent;
};

} // namespace Raytracer

#endif // RAYTRACER_OCTREE_H
