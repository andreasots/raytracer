#include "raytracer/octree.h"
#include <gmtl/Intersection.h>

inline bool within(const gmtl::AABox<FLOAT> &box1,
                   const gmtl::AABox<FLOAT> &box2)
{
    if(box1.getMax()[0] < box2.getMax()[0])
        return false;
    if(box1.getMax()[1] < box2.getMax()[1])
        return false;
    if(box1.getMax()[2] < box2.getMax()[2])
        return false;
    if(box1.getMin()[0] > box2.getMin()[0])
        return false;
    if(box1.getMin()[1] > box2.getMin()[1])
        return false;
    if(box1.getMin()[2] > box2.getMin()[2])
        return false;
    return true;
}

inline gmtl::AABox<FLOAT> aabb(const gmtl::Sphere<FLOAT> &s)
{
    gmtl::Point<FLOAT, 3> max, min;
    for(size_t i = 0; i < 3U; i++)
    {
        min[i] = s.getCenter()[i]-s.getRadius();
        max[i] = s.getCenter()[i]+s.getRadius();
    }
    return gmtl::AABox<FLOAT>(min, max);
}

inline gmtl::AABox<FLOAT> aabb(const gmtl::Tri<FLOAT> &tri)
{
    gmtl::Point<FLOAT, 3> max, min;

    for(size_t i = 0; i < 3U; i++)
    {
        min[i] = std::min(tri[0][i], std::min(tri[1][i], tri[2][i]));
        max[i] = std::max(tri[0][i], std::max(tri[1][i], tri[2][i]));
    }

    return gmtl::AABox<FLOAT>(min, max);
}

namespace Raytracer {

Octree::Octree()
{
}

Octree::Octree(const gmtl::Point<FLOAT, 3>& min, const gmtl::Point<FLOAT, 3>& max, Octree *parent):
    objects(), m_subnodes({NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}),
    AABox(min, max), m_parent(parent)
{
    setEmpty(true);
}

Octree::~Octree()
{
    for(size_t i = 0; i < 8; i++)
        delete m_subnodes[i];
}

bool Octree::add(const gmtl::Tri<FLOAT> &tri, const size_t &id)
{
    if(within(*this, aabb(tri)))
    {
        createSubnodes();
        for(size_t i = 0; i < 8; i++)
            if(m_subnodes[i]->add(tri, id))
                return true;
        objects.push_back(std::make_pair(TRI, id));
        setEmpty(false);
        return true;
    }
    return false;
}

bool Octree::add(const gmtl::Sphere<FLOAT> &sphere, const size_t &id)
{
    if(within(*this, aabb(sphere)))
    {
        createSubnodes();
        for(size_t i = 0; i < 8; i++)
            if(m_subnodes[i]->add(sphere, id))
                return true;
        objects.push_back(std::make_pair(SPHERE, id));
        setEmpty(false);
        return true;
    }
    return false;
}

void Octree::prune()
{
    for(size_t i = 0; i < 8; i++)
    {
        if(m_subnodes[i]->isEmpty())
        {
            delete m_subnodes[i];
            m_subnodes[i] = NULL;
            continue;
        }
    }
}

bool Octree::intersect(const gmtl::Ray<FLOAT> &r,
                       std::vector<Octree*> &nodes)
{
    unsigned numHits;
    FLOAT tIn, tOut;
    if(!gmtl::intersect(*this, r, numHits, tIn, tOut))
        return false;
    nodes.push_back(this);
    for(size_t i = 0; i < 8; i++)
        if(m_subnodes[i])
            m_subnodes[i]->intersect(r, nodes);
    return true;
}

void Octree::createSubnodes()
{
    FLOAT xHeight = (mMax[0]-mMin[0])/2, x[] = {mMin[0], mMin[0]+xHeight};
    FLOAT yHeight = (mMax[1]-mMin[1])/2, y[] = {mMin[1], mMin[1]+yHeight};
    FLOAT zHeight = (mMax[2]-mMin[2])/2, z[] = {mMin[2], mMin[2]+zHeight};
    for(size_t i = 0; i < 8; i++)
    {
        if(m_subnodes[i])
            continue;
        gmtl::Point<FLOAT, 3> min, max;
        // x-axis
        min[0] = x[i&1];
        max[0] = x[i&1]+xHeight;
        // y-axis
        min[1] = y[i&2>>1];
        max[1] = y[i&2>>1]+yHeight;
        // z-axis
        min[2] = z[i&4>>2];
        max[2] = z[i&4>>2]+zHeight;
        m_subnodes[i] = new Octree(min, max, this);
    }
}

} // namespace Raytracer
