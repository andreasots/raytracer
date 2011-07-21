#include "raytracer/octree.h"

template <class T> inline bool within(const gmtl::AABox<T> &box1,
                                      const gmtl::AABox<T> &box2)
{
    if(box1.getMax()[0] > box2.getMax()[0])
        return false;
    if(box1.getMax()[1] > box2.getMax()[1])
        return false;
    if(box1.getMax()[2] > box2.getMax()[2])
        return false;
    if(box1.getMin()[0] < box2.getMin()[0])
        return false;
    if(box1.getMin()[1] < box2.getMin()[1])
        return false;
    if(box1.getMin()[2] < box2.getMin()[2])
        return false;
    return true;
}

namespace Raytracer {

Octree::Octree(const Point<DATA_TYPE, 3>& min, const Point<DATA_TYPE, 3>& max):
    objects(), m_subnodes({NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}),
    AABox(min, max)
{
    setEmpty(true);
}

Octree::~Octree()
{

}

bool Octree::add(gmtl::Tri<FLOAT> tri, size_t id)
{

}

bool Octree::add(gmtl::Sphere<FLOAT> sphere, size_t id);

} // namespace Raytracer
