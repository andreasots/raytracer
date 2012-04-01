#ifndef RAYTRACER_OCTREE_NODE_H
#define RAYTRACER_OCTREE_NODE_H

#include "raytracer/object.h"

#include <SIMD/AABox.h>
#include <SIMD/Ray.h>

#include <tuple>
#include <vector>

namespace Raytracer {
namespace Octree {

class Node
{
    public:
        /** Default constructor */
        Node();
        /** Default destructor */
        virtual ~Node();
        bool add(const SIMD::AABox &bounds, const std::tuple<SIMD::AABox, std::size_t> &o);
        RT_FLOAT intersect(const SIMD::Ray &r, const SIMD::AABox &dist, std::size_t &id,
                           RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max,
                           const std::vector<Raytracer::Object*> &objects,
                           unsigned long long &intersections, unsigned long long &hits);
        void optimize();
    protected:
        Node *subnodes[8];
        std::vector<std::size_t> m_objects;
    private:
};

} // namespace Octree
} // namespace Raytracer

#endif // RAYTRACER_OCTREE_NODE_H
