#ifndef SIMDVEC_H
#define SIMDVEC_H

#include <cstdlib>
#include <xmmintrin.h>
#include <smmintrin.h>
#include <stdexcept>
#include <cmath>
#include <ostream>
#include <iostream>
namespace SIMD
{
class Vec
{
	__m128 mData;
	public:
		Vec() throw(): mData(_mm_setzero_ps())
		{
		}

		Vec(float x, float y, float z, float w = 0) throw(): mData(_mm_set_ps(x, y, z, w))
		{
		}

		Vec(__m128 _data) throw(): mData(_data)
		{
		}

		inline Vec operator+(const Vec &v) const throw()
		{
			return _mm_add_ps(mData, v.mData);
		}

		inline Vec &operator+=(const Vec &v) throw()
		{
			mData = _mm_add_ps(mData, v.mData);
			return *this;
		}

        inline Vec operator-() const throw()
        {
            return _mm_sub_ps(_mm_setzero_ps(), mData);
        }

		inline Vec operator-(const Vec &v) const throw()
		{
			return _mm_sub_ps(mData, v.mData);
		}

		inline Vec &operator-=(const Vec &v) throw()
		{
			mData = _mm_sub_ps(mData, v.mData);
			return *this;
		}

		inline Vec operator*(float f) const throw()
		{
			return _mm_mul_ps(mData, _mm_load1_ps(&f));
		}

		inline Vec &operator*=(float f) throw()
		{
			mData = _mm_mul_ps(mData, _mm_load1_ps(&f));
			return *this;
		}

		inline Vec operator/(float f) const throw()
		{
			return _mm_div_ps(mData, _mm_load1_ps(&f));
		}

		inline Vec &operator/=(float f) throw()
		{
			mData = _mm_div_ps(mData, _mm_load1_ps(&f));
			return *this;
		}

		inline float operator[](size_t i) const throw(std::out_of_range)
		{
			if(i & ~3U)
				throw std::out_of_range("Vec::operator[]");
			return reinterpret_cast<const float*>(&mData)[3-i];
		}

		inline float dot(const Vec &v) const throw()
		{
			return _mm_cvtss_f32(_mm_dp_ps(mData, v.mData, 0xF1));
		}

 		inline Vec cross(const Vec &v) const throw()
        {
			return _mm_sub_ps(
                     _mm_mul_ps(_mm_shuffle_ps(mData, mData,
                                               _MM_SHUFFLE(2, 1, 3, 0)),
                                _mm_shuffle_ps(v.mData, v.mData,
                                               _MM_SHUFFLE(1, 3, 2, 0))),
                     _mm_mul_ps(_mm_shuffle_ps(mData, mData,
                                               _MM_SHUFFLE(1, 3, 2, 0)),
                                _mm_shuffle_ps(v.mData, v.mData,
                                               _MM_SHUFFLE(2, 1, 3, 0))));
		}

		inline float length() const throw()
		{
			return std::sqrt(dot(mData));
		}

		inline float length2() const throw()
		{
		    return dot(mData);
		}

		inline void normalize() throw()
		{
			mData = _mm_div_ps(mData, _mm_sqrt_ps(_mm_dp_ps(mData, mData, 0xFF)));
		}

		inline __m128 data() const throw()
		{
		    return mData;
		}
		friend inline std::ostream &operator<<(std::ostream &out, const Vec &v);
};

inline Vec operator*(float f, Vec v)
{
	return v*f;
}

inline std::ostream &operator<<(std::ostream &out, const Vec &v)
{
	out << '(' << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ')';
	return out;
}
}
#endif
