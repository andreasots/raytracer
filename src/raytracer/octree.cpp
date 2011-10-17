#include "raytracer/octree.h"
#include <stdexcept>
#include <iostream>
#define RT_INF (static_cast<RT_FLOAT>(HUGE_VAL))

namespace Raytracer {

Octree::Octree(const SIMD::Point& _min, const SIMD::Point& _max, Octree *parent):
    SIMD::AABox(_min, _max), empty(true), m_objects(), m_subnodes({NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}),
    m_parent(parent)
{
}

Octree::~Octree()
{
    for(size_t i = 0; i < 8; i++)
        delete m_subnodes[i];
}

bool Octree::add(Object *o, const size_t &id)
{
    if(encloses(o->bounds()))
    {
        createSubnodes();
        for(size_t i = 0; i < 8; i++)
            if(m_subnodes[i]->add(o, id))
                return true;
        m_objects.push_back(id);
        return true;
    }
    return false;
}

void Octree::prune(const std::vector<Object *> &objects)
{
    for(size_t i = 0; i < 8; i++)
        if(m_subnodes[i])
            m_subnodes[i]->prune(objects);

    size_t nodes = 8;
    for(size_t i = 0; i < 8; i++)
    {
        if(m_subnodes[i] && m_subnodes[i]->empty)
        {
            delete m_subnodes[i];
            m_subnodes[i] = NULL;
            nodes--;
        }
    }

    if(m_parent && nodes == 1 && m_objects.size() == 0)
    {
        size_t from, to;
        for(size_t i = 0; i < 8; i++)
        {
            if(m_parent->m_subnodes[i] == this)
                to = i;
            if(m_subnodes[i])
                from = i;
        }
        m_parent->m_subnodes[to] = m_subnodes[from];
        m_subnodes[from] = NULL;
        delete this;
        return;
    }
    RT_FLOAT _min[] = {RT_INF, RT_INF, RT_INF};
    RT_FLOAT _max[] = {-RT_INF, -RT_INF, -RT_INF};
    for(size_t i = 0; i < 8; i++)
    {
        if(m_subnodes[i])
        {
                _max[0] = std::max(m_subnodes[i]->max()[0], _max[0]);
                _min[0] = std::min(m_subnodes[i]->min()[0], _min[0]);
                _max[1] = std::max(m_subnodes[i]->max()[1], _max[1]);
                _min[1] = std::min(m_subnodes[i]->min()[1], _min[1]);
                _max[2] = std::max(m_subnodes[i]->max()[2], _max[2]);
                _min[2] = std::min(m_subnodes[i]->min()[2], _min[2]);
        }
    }

    setMin(SIMD::Point(_min[0], _min[1], _min[2]));
    setMax(SIMD::Point(_max[0], _max[1], _max[2]));
    for(size_t i = 0; i < m_objects.size(); i++)
        extend(objects[m_objects[i]]->bounds());
}

bool Octree::intersect(const SIMD::Ray &r, RT_FLOAT &ret, size_t &id, RT_FLOAT &u, RT_FLOAT &v,
                       const std::vector<Object *> &objects,
                       unsigned long long &hits, unsigned long long &intersections)
{
    RT_FLOAT tIn, tOut;
    if(!reinterpret_cast<SIMD::AABox*>(this)->intersect(r, tIn, tOut) || tIn >= ret || tOut <= 0)
        return false;

    for(auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        RT_FLOAT U, V;
        RT_FLOAT t = objects[*i]->intersect(r, U, V);
        if(t < ret)
        {
            ret = t;
            id = *i;
            u = U;
            v = V;
            hits++;
        }
        intersections++;
    }

    for(size_t i = 0; i < 8; i++)
        if(m_subnodes[i])
            m_subnodes[i]->intersect(r, ret, id, u, v, objects, hits, intersections);

    return true;
}

void Octree::createSubnodes()
{
    RT_FLOAT xHeight = (max()[0]-min()[0])/2, x[] = {min()[0], min()[0]+xHeight};
    RT_FLOAT yHeight = (max()[1]-min()[1])/2, y[] = {min()[1], min()[1]+yHeight};
    RT_FLOAT zHeight = (max()[2]-min()[2])/2, z[] = {min()[2], min()[2]+zHeight};
    for(size_t i = 0; i < 8; i++)
    {
        if(m_subnodes[i])
            continue;
        RT_FLOAT _min[3], _max[3];
        // x-axis
        _min[0] = x[i&1];
        _max[0] = x[i&1]+xHeight;
        // y-axis
        _min[1] = y[i&2>>1];
        _max[1] = y[i&2>>1]+yHeight;
        // z-axis
        _min[2] = z[i&4>>2];
        _max[2] = z[i&4>>2]+zHeight;
        m_subnodes[i] = new Octree(SIMD::Point(_min[0], _min[1], _min[2]), SIMD::Point(_max[0], _max[1], _max[2]), this);
    }
}

} // namespace Raytracer
