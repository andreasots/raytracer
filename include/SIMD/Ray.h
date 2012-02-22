#ifndef SIMDRAY_H_INCLUDED
#define SIMDRAY_H_INCLUDED

#include "SIMD/Vec.h"
#include "SIMD/Point.h"

namespace SIMD
{
    struct Ray
    {
        Point origin;
        Vec direction, inv_d;

        Ray(Point o, Vec d): direction(d), origin(o)
        {
            inv_d = _mm_rcp_ps(d.data());
            inv_d = _mm_mul_ps(inv_d.data(), _mm_sub_ps(_mm_set1_ps(2), _mm_mul_ps(d.data(), inv_d.data())));
        }
    };
}

#endif // SIMDRAY_H_INCLUDED
