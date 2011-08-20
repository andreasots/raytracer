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
        FLOAT intersect(const gmtl::Ray<FLOAT> &r,
                        const std::vector<Object*> &objects,
                        unsigned long long &tests,
                        unsigned long long &hits);
        void generate(const std::vector<Object*> &objects);
    protected:
        FLOAT cost();
        size_t data;
        FLOAT pos;
    private:
};

}

#endif // KDTREE_H
