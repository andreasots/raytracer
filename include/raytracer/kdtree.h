#ifndef RAYTRACER_KDTREE_H
#define RAYTRACER_KDTREE_H

#include "raytracer/object.h"
#include <gmtl/Ray.h>

namespace Raytracer
{

class KdTree
{
    public:
        /** Default constructor */
        KdTree();
        RT_FLOAT intersect(const gmtl::Ray<RT_FLOAT> &r,
                        const std::vector<Object*> &objects,
                        unsigned long long &tests,
                        unsigned long long &hits);
        void generate(const std::vector<Object*> &objects);
    protected:
        RT_FLOAT cost();
        size_t data;
        RT_FLOAT pos;
    private:
};

}

#endif // KDTREE_H
