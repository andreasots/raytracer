#ifndef SIMDRAY_H_INCLUDED
#define SIMDRAY_H_INCLUDED

#include "SIMD/Vec.h"
#include "SIMD/Point.h"

/* An Efficient and Robust Ray-Box Intersection Algorithm */
/* Amy Williams, Steve Barrus, R. Keith Morley, Peter Shirley */


namespace SIMD
{
    struct Ray
    {
        Vec direction, invd;
        Point origin;
        int sign[4];

        Ray(Point o, Vec d): direction(d), invd(), origin(o), sign()
        {
            invd = _mm_rcp_ps(d.data());
            sign[0] = (invd[0] < 0);
            sign[1] = (invd[1] < 0);
            sign[2] = (invd[2] < 0);
            sign[3] = (invd[3] < 0);
        }
    };
}

#endif // SIMDRAY_H_INCLUDED
