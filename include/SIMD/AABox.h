#ifndef SIMD_AABOX_H
#define SIMD_AABOX_H
#include "Ray.h"
#include <xmmintrin.h>
#include <cstring>
/* An Efficient and Robust Ray-Box Intersection Algorithm */
/* Amy Williams, Steve Barrus, R. Keith Morley, Peter Shirley */
namespace SIMD
{
    class AABox
    {
        Point mBounds[2];
        public:
        AABox(Point _min, Point _max)
        {
            mBounds[0] = _min;
            mBounds[1] = _max;
        }

        virtual ~AABox()
        {
        }


        bool encloses(const AABox &box) const throw()
        {
            __m128 res = _mm_cmple_ps(box.mBounds[0].data(), mBounds[0].data());
            if(std::memchr(reinterpret_cast<void*>(&res), 0, sizeof(__m128)) != NULL)
                return false;
            res = _mm_cmpge_ps(box.mBounds[1].data(), mBounds[1].data());
            if(std::memchr(reinterpret_cast<void*>(&res), 0, sizeof(__m128)) != NULL)
                return false;
            return true;
        }

        void extend(const AABox &box) throw()
        {
            float _min[] = {mBounds[0][0], mBounds[0][1], mBounds[0][2]};
            float _max[] = {mBounds[1][0], mBounds[1][1], mBounds[1][2]};

            _max[0] = std::max(box.max()[0], _max[0]);
            _min[0] = std::min(box.min()[0], _min[0]);
            _max[1] = std::max(box.max()[1], _max[1]);
            _min[1] = std::min(box.min()[1], _min[1]);
            _max[2] = std::max(box.max()[2], _max[2]);
            _min[2] = std::min(box.min()[2], _min[2]);

            mBounds[0] = Point(_min[0], _min[1], _min[2]);
            mBounds[1] = Point(_max[0], _max[1], _max[2]);
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
/*            __m128 res = _mm_set_ps(mBounds[r.sign[0]][0], mBounds[1-r.sign[0]][0], mBounds[r.sign[1]][1], mBounds[1-r.sign[1]][1]);
            res = _mm_sub_ps(res, _mm_set_ps(r.origin[0], r.origin[0], r.origin[1], r.origin[1]));
            res = _mm_mul_ps(res, _mm_set_ps(r.invd[0], r.invd[0], r.invd[1], r.invd[1]));
            float tmin = _mm_cvtss_f32(_mm_shuffle_ps(res, res, 0));
            float tmax = _mm_cvtss_f32(_mm_shuffle_ps(res, res, 1));
            float tymin = _mm_cvtss_f32(_mm_shuffle_ps(res, res, 2));
            float tymax = _mm_cvtss_f32(_mm_shuffle_ps(res, res, 3));
            float tmin = (mBounds[r.sign[0]][0] - r.origin[0]) * r.invd[0];
            float tmax = (mBounds[1-r.sign[0]][0] - r.origin[0]) * r.invd[0];
            float tymin = (mBounds[r.sign[1]][1] - r.origin[1]) * r.invd[1];
            float tymax = (mBounds[1-r.sign[1]][1] - r.origin[1]) * r.invd[1];
            if((tmin > tymax)||(tymin > tmax))
                return false;
            if(tymin > tmin)
                tmin = tymin;
            if(tymax < tmax)
                tmax = tymax;
            res = _mm_set_ps(mBounds[r.sign[2]][2], mBounds[1-r.sign[2]][2], 0, 0);
            res = _mm_sub_ps(res, _mm_set_ps(r.origin[2], r.origin[2], 0, 0));
            res = _mm_mul_ps(res, _mm_set_ps(r.invd[2], r.invd[2], 0, 0));
            float tzmin = _mm_cvtss_f32(_mm_shuffle_ps(res, res, 0));
            float tzmax = _mm_cvtss_f32(_mm_shuffle_ps(res, res, 1));
            float tzmin = (mBounds[r.sign[2]][2] - r.origin[2]) * r.invd[2];
            float tzmax = (mBounds[1-r.sign[2]][2] - r.origin[2]) * r.invd[2];
            if((tmin > tzmax)||(tzmin > tmax))
                return false;
            if(tzmin > tmin)
                tmin = tzmin;
            if(tzmax < tmax)
                tmax = tzmax;
            t0 = tmin;
            t1 = tmax;
            return true;*/
	    return false;
        }
    };
}

#endif
