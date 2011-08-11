#ifndef RAYTRACER_OCTREE_H
#define RAYTRACER_OCTREE_H

#include <gmtl/AABox.h>
#include <gmtl/Ray.h>
#include <vector>

#include "raytracer/object.h"

namespace Raytracer {

class Octree: public gmtl::AABox<FLOAT>
{
    public:
        Octree(const gmtl::Point<FLOAT, 3>& min,
               const gmtl::Point<FLOAT, 3>& max, Octree *parent);
        virtual ~Octree();
        bool add(Object *o, const size_t &id);
        bool intersect(const gmtl::Ray<FLOAT> &r, FLOAT &ret, size_t &id,
                       const std::vector<Object *> &objects,
                       unsigned long long &hits, unsigned long long &intersections);
        void prune();
        void createSubnodes();
    protected:
        std::vector<size_t> m_objects;
        Octree *m_subnodes[8];
        Octree *m_parent;
};

} // namespace Raytracer

#endif // RAYTRACER_OCTREE_H
