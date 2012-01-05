#ifndef SIMDRAY_H_INCLUDED
#define SIMDRAY_H_INCLUDED

#include "SIMD/Vec.h"
#include "SIMD/Point.h"

namespace SIMD
{
    struct Ray
    {
        Vec direction;
        Point origin;

        Ray(Point o, Vec d): direction(d), origin(o)
        {
        }
    };
}

#endif // SIMDRAY_H_INCLUDED
