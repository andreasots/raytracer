#ifndef RAYTRACER_OCTREE_H
#define RAYTRACER_OCTREE_H

#include <gmtl/AABox.h>
#include <gmtl/Tri.h>
#include <gmtl/Sphere.h>
#include <gmtl/Ray.h>
#include <gmtl/Intersection.h>
#include <vector>

namespace Raytracer {

enum ObjectType
{
    PLANE,
    SPHERE,
    TRI
};

class Octree: public gmtl::AABox<FLOAT>
{
    public:
        Octree();
        Octree(const gmtl::Point<FLOAT, 3>& min,
               const gmtl::Point<FLOAT, 3>& max, Octree *parent);
        ~Octree();
        bool add(const gmtl::Tri<FLOAT> &tri, const size_t &id);
        bool add(const gmtl::Sphere<FLOAT> &sphere, const size_t &id);
        bool intersect(const gmtl::Ray<FLOAT> &r, std::vector<Octree*> &nodes);
        void prune();
        void createSubnodes();
        std::vector<std::pair<ObjectType, size_t>> objects;
    protected:
        Octree *m_subnodes[8];
        Octree *m_parent;
};

class cmpOctree
{
    public:
    gmtl::Ray<FLOAT> r;
    cmpOctree(gmtl::Ray<FLOAT> _r): r(_r)
    {
    }

    bool operator()(Octree* a, Octree* b)
    {
        FLOAT tInA, tOutA, tInB, tOutB;
        gmtl::intersectAABoxRay(*a, r, tInA, tOutA);
        gmtl::intersectAABoxRay(*b, r, tInB, tOutB);
        return tInA < tInB;
    }
};
} // namespace Raytracer

#endif // RAYTRACER_OCTREE_H
