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

        SIMD::Vec operator[](size_t i) throw(std::out_of_range)
        {
            if(i >= 4)
                throw std::out_of_range("SIMD::Matrix::operator[]");
            return mData[i];
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
            Vec r[4] = {mData[0], mData[1], mData[2], mData[3]};
            Matrix m(_mm_set_ps(r[0][0], r[1][0], r[2][0], r[3][0]),
                     _mm_set_ps(r[0][1], r[1][1], r[2][1], r[3][1]),
                     _mm_set_ps(r[0][2], r[1][2], r[2][2], r[3][2]),
                     _mm_set_ps(r[0][3], r[1][3], r[2][3], r[3][3]));
            return Vec(v.dot(m.mData[0]), v.dot(m.mData[1]), v.dot(m.mData[2]), v.dot(m.mData[3]));
        }

        Point operator*(const Point &p) const throw()
        {
            Vec v(p.data());
            return Point(((*this)*v).data());
        }

        void transpose() throw()
        {
            _MM_TRANSPOSE4_PS(mData[0], mData[1], mData[2], mData[3]);
        }

        static Matrix identity()
        {
            return Matrix(_mm_set_ps(1, 0, 0, 0), _mm_set_ps(0, 1, 0, 0), _mm_set_ps(0, 0, 1, 0), _mm_set_ps(0, 0, 0, 1));
        }
        friend inline std::ostream &operator<<(std::ostream &out, const Matrix &v);
    };

    inline std::ostream &operator<<(std::ostream &out, const Matrix &v)
    {
        out << "Matrix(" << Vec(v.mData[0]) << std::endl;
        out << "       " << Vec(v.mData[1]) << std::endl;
        out << "       " << Vec(v.mData[2]) << std::endl;
        out << "       " << Vec(v.mData[3]) << ")"<< std::endl;
        return out;
    }
}
#endif
