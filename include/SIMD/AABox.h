#ifndef SIMD_AABOX_H
#define SIMD_AABOX_H
#include "Ray.h"
#include <xmmintrin.h>
#include <cstring>

namespace SIMD
{
    class AABox
    {
        Point mBounds[2] __attribute__((aligned(16)));
        public:
        AABox(Point _min = Point(), Point _max = Point())
        {
            mBounds[0] = _min;
            mBounds[1] = _max;
        }

        virtual ~AABox()
        {
        }


        bool encloses(const AABox &box) const throw()
        {
            __m128 res = _mm_cmplt_ps(box.mBounds[0].data(), mBounds[0].data());
            if(_mm_movemask_ps(res) & 0xE)
                return false;
            res = _mm_cmpgt_ps(box.mBounds[1].data(), mBounds[1].data());
            if(_mm_movemask_ps(res) & 0xE)
                return false;
            return true;
        }

        void extend(const AABox &box) throw()
        {
            mBounds[0] = _mm_min_ps(mBounds[0].data(), box.mBounds[0].data());
            mBounds[1] = _mm_max_ps(mBounds[1].data(), box.mBounds[1].data());
        }

        bool empty() const throw()
        {
            SIMD::Vec d = mBounds[1] - mBounds[0];
            if(d.dot(d) == 0)
                return true;
            return false;
        }

        Point min() const throw()
        {
            return mBounds[0];
        }

        Point max() const throw()
        {
            return mBounds[1];
        }

        void setMin(Point p)
        {
            mBounds[0] = p;
        }

        void setMax(Point p)
        {
            mBounds[1] = p;
        }

        bool intersect(const Ray &r, RT_FLOAT &t0, RT_FLOAT &t1) const
        {
            __m128 T1, T2, temp;

            T1 = _mm_mul_ps((mBounds[0]-r.origin).data(), r.inv_d.data());
            T2 = _mm_mul_ps((mBounds[1]-r.origin).data(), r.inv_d.data());
            temp = T1;
            T1 = _mm_min_ps(T1, T2);
            T2 = _mm_max_ps(temp, T2);

            SIMD::Vec v = T1;

            t0 = std::max(v[0], std::max(v[1], v[2]));
            v = T2;
            t1 = std::min(v[0], std::min(v[1], v[2]));

            return t0 < t1 && t1 > 0;
        }
    };
}

#endif
