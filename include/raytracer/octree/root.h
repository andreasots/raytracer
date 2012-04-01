#ifndef RAYTRACER_OCTREE_ROOT_H
#define RAYTRACER_OCTREE_ROOT_H

#include <SIMD/AABox.h>
#include <SIMD/Ray.h>
#include <SIMD/Point.h>
#include "raytracer/object.h"
#include "raytracer/octree/node.h"

#include <vector>
#include <tuple>

namespace Raytracer {
namespace Octree {

class Root
{
    public:
        Root();
        virtual ~Root();

        RT_FLOAT intersect(const SIMD::Ray &r, std::size_t &id, RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max,
                           const std::vector<Raytracer::Object*> &objects,
                           unsigned long long &intersections, unsigned long long &hits);

        void addCylinder(const SIMD::Point &a, const SIMD::Point &b, const RT_FLOAT r, std::size_t id);
        void addQuad(const SIMD::Point &v00, const SIMD::Point &v01, const SIMD::Point &v11, const SIMD::Point &v10, std::size_t id);
        void addSphere(const SIMD::Point &c, RT_FLOAT r, std::size_t id);
        void addTri(const SIMD::Point &a, const SIMD::Point &b, const SIMD::Point &c, std::size_t id);

        void rebuild();

    protected:
        SIMD::AABox m_bounds;
        std::vector<std::tuple<SIMD::AABox, std::size_t>> m_objects;
        Node *tree;
    private:
};

} // namespace Octree
} // namespace Raytracer

#endif // RAYTRACER_OCTREE_ROOT_H
