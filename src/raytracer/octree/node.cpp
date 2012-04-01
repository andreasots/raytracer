#include "raytracer/octree/node.h"

#include "raytracer/allocator.h"

#include <SIMD/AABox.h>

#include <iostream>

namespace Raytracer {
namespace Octree {

Node::Node()
{
    for(int i = 0; i < 8; i++)
        subnodes[i] = NULL;
}

Node::~Node()
{
    for(int i = 0; i < 8; i++)
        deallocate<Node, 16>(subnodes[i], 1);
}

bool Node::add(const SIMD::AABox &bounds, const std::tuple<SIMD::AABox, std::size_t> &o)
{
    if(!bounds.encloses(std::get<0>(o)))
        return false;

    bool ret = false;

    SIMD::Vec avg = 0.5*(SIMD::Vec(bounds.min().data())+SIMD::Vec(bounds.max().data()));
    SIMD::AABox bbox;

    for(int i = 0; !ret && i < 8; i++)
    {
        if(!subnodes[i])
            subnodes[i] = new (allocate<Node, 16>(1)) Node;

        bbox.setMin(SIMD::Point((i & 1 ? bounds.min()[0] : avg[0]),
                                (i & 2 ? bounds.min()[1] : avg[1]),
                                (i & 4 ? bounds.min()[2] : avg[2])));
        bbox.setMax(SIMD::Point((i & 1 ? avg[0] : bounds.max()[0]),
                                (i & 2 ? avg[1] : bounds.max()[1]),
                                (i & 4 ? avg[2] : bounds.max()[2])));

        ret = subnodes[i]->add(bbox, o);
    }

    if(!ret)
        m_objects.push_back(std::get<1>(o));

    return true;
}

RT_FLOAT Node::intersect(const SIMD::Ray &r, const SIMD::AABox &dist, std::size_t &id,
                   RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max,
                   const std::vector<Raytracer::Object*> &objects,
                   unsigned long long &intersections, unsigned long long &hits)
{
    SIMD::AABox bbox;
    SIMD::Vec avg = 0.5*(SIMD::Vec(dist.min().data())+SIMD::Vec(dist.min().data()));

    bbox.setMin(SIMD::Point(_mm_min_ps(dist.min().data(), dist.max().data())));
    bbox.setMax(SIMD::Point(_mm_max_ps(dist.min().data(), dist.max().data())));

    RT_FLOAT tIn = std::max(bbox.min()[0], std::max(bbox.min()[1], bbox.min()[2]));
    RT_FLOAT tOut = std::min(bbox.max()[0], std::min(bbox.max()[1], bbox.max()[2]));
//    if(tIn > max || tOut < 0 || tIn > tOut)
//        return max;

    RT_FLOAT ret = max;

    for(auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        RT_FLOAT U, V;
        RT_FLOAT T = objects[*i]->intersect(r, U, V);
        if(T < ret)
        {
            ret = T;
            id = *i;
            u = U;
            v = V;
            hits++;
        }
        intersections++;
    }

    for(int i = 0; i < 8; i++)
    {
        if(!subnodes[i])
            continue;

        bbox.setMin(SIMD::Point((i & 1 ? dist.min()[0] : avg[0]),
                                (i & 2 ? dist.min()[1] : avg[1]),
                                (i & 4 ? dist.min()[2] : avg[2])));
        bbox.setMax(SIMD::Point((i & 1 ? avg[0] : dist.max()[0]),
                                (i & 2 ? avg[1] : dist.max()[1]),
                                (i & 4 ? avg[2] : dist.max()[2])));
        std::size_t ID;
        RT_FLOAT U, V, T = subnodes[i]->intersect(r, bbox, ID, U, V, ret, objects, intersections, hits);
        if(T < ret)
        {
            ret = T;
            id = ID;
            u = U;
            v = V;
        }
    }

    return ret;
}

void Node::optimize()
{
    for(int i = 0; i < 8; i++)
    {
        if(!subnodes[i])
            continue;
        if(!subnodes[i]->m_objects.size())
        {
            int j;
            for(j = 0; j < 8; j++)
                if(subnodes[i]->subnodes[j])
                    break;
            if(j == 8)
            {
                deallocate<Node, 16>(subnodes[i], 1);
                subnodes[i] = NULL;
                continue;
            }
        }
        subnodes[i]->optimize();
    }
}

} // namespace Octree
} // namespace Raytracer
