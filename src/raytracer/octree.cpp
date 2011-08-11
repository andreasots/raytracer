#include "raytracer/octree.h"
#include <gmtl/Intersection.h>
#include <stdexcept>
#include <iostream>

#include <gmtl/Containment.h>

namespace Raytracer {

Octree::Octree(const gmtl::Point<FLOAT, 3>& min, const gmtl::Point<FLOAT, 3>& max, Octree *parent):
    m_objects(), m_subnodes({NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}),
    // FIXME: Compiler is fucked up
    /*AABox(min, max),*/ m_parent(parent)
{
    // FIXME: Compiler is fucked up
    setMin(min);
    setMax(max);
    setEmpty(true);
}

Octree::~Octree()
{
    for(size_t i = 0; i < 8; i++)
        delete m_subnodes[i];
}

bool Octree::add(Object *o, const size_t &id)
{
    bool e = isEmpty();
    setEmpty(false);
    if(gmtl::isInVolume(*this, o->bounds()))
    {
        createSubnodes();
        for(size_t i = 0; i < 8; i++)
            if(m_subnodes[i]->add(o, id))
                return true;
        m_objects.push_back(id);
        setEmpty(false);
        return true;
    }
    setEmpty(e);
    return false;
}

void Octree::prune()
{
    for(size_t i = 0; i < 8; i++)
    {
        if(m_subnodes[i] && m_subnodes[i]->isEmpty())
        {
            delete m_subnodes[i];
            m_subnodes[i] = NULL;
            continue;
        }
    }
}

bool Octree::intersect(const gmtl::Ray<FLOAT> &r, FLOAT &ret, size_t &id,
                       const std::vector<Object *> &objects,
                       unsigned long long &hits, unsigned long long &intersections)
{
    FLOAT tIn, tOut;
    if(!gmtl::intersectAABoxRay(*this, r, tIn, tOut) || tIn >= ret || tOut <= 0)
        return false;

    for(auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        FLOAT t = objects[*i]->intersect(r);
        if(t < ret)
        {
            ret = t;
            id = *i;
            hits++;
        }
        intersections++;
    }

    for(size_t i = 0; i < 8; i++)
        if(m_subnodes[i])
            m_subnodes[i]->intersect(r, ret, id, objects, hits, intersections);

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
