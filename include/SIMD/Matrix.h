#ifndef SIMD_MATRIX_H
#define SIMD_MATRIX_H
#include <xmmintrin.h>
#include "Vec.h"
#include "Point.h"

namespace SIMD
{
    class Matrix
    {
        __m128 mData[4];
    public:
        Matrix() throw(): mData({_mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps()})
        {
        }

        Matrix(__m128 d1, __m128 d2, __m128 d3, __m128 d4) throw(): mData({d1, d2, d3, d4})
        {
        }

        Matrix(float *f) throw(): mData({_mm_loadu_ps(f), _mm_loadu_ps(f+4), _mm_loadu_ps(f+8), _mm_loadu_ps(f+12)})
        {
        }

        Matrix(const Matrix &m) throw(): mData({m.mData[0], m.mData[1], m.mData[2], m.mData[3]})
        {
        }

        Matrix &operator=(const Matrix &m) throw()
        {
            mData[0] = m.mData[0];
            mData[1] = m.mData[1];
            mData[2] = m.mData[2];
            mData[3] = m.mData[3];
            return *this;
        }

        Matrix operator+(const Matrix &m) const throw()
        {
            return Matrix(_mm_add_ps(mData[0], m.mData[0]),
                          _mm_add_ps(mData[1], m.mData[1]),
                          _mm_add_ps(mData[2], m.mData[2]),
                          _mm_add_ps(mData[3], m.mData[3]));
        }

        Matrix operator-(const Matrix &m) const throw()
        {
            return Matrix(_mm_sub_ps(mData[0], m.mData[0]),
                          _mm_sub_ps(mData[1], m.mData[1]),
                          _mm_sub_ps(mData[2], m.mData[2]),
                          _mm_sub_ps(mData[3], m.mData[3]));
        }

        Matrix operator*(const Matrix &m) const throw()
        {
            Matrix mT = m, ret;
            _MM_TRANSPOSE4_PS(mT.mData[0], mT.mData[1], mT.mData[2], mT.mData[3]);
            for(int x = 0; x < 4; x++)
                ret.mData[x] = _mm_set_ps(Vec(mData[x]).dot(mT.mData[0]),
                                          Vec(mData[x]).dot(mT.mData[1]),
                                          Vec(mData[x]).dot(mT.mData[2]),
                                          Vec(mData[x]).dot(mT.mData[3]));
            return ret;
        }

        Vec operator*(const Vec &v) const throw()
        {
            return Vec(v.dot(mData[0]), v.dot(mData[1]), v.dot(mData[2]), v.dot(mData[3]));
        }

        Point operator*(const Point &p) const throw()
        {
            Vec v(p.data());
            return Point(v.dot(mData[0]), v.dot(mData[1]), v.dot(mData[2]), v.dot(mData[3]));
        }

        void transpose() throw()
        {
            _MM_TRANSPOSE4_PS(mData[0], mData[1], mData[2], mData[3]);
        }
    };
}
#endif
