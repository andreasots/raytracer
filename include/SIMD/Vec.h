#ifndef SIMDVEC_H
#define SIMDVEC_H

#include <cstdlib>
#include <xmmintrin.h>
#include <smmintrin.h>
#include <stdexcept>
#include <cmath>
#include <ostream>
namespace SIMD
{
class Vec
{
	__m128 mData;
	public:
		Vec() throw(): mData(_mm_setzero_ps())
		{
		}

		Vec(float x, float y, float z, float w = 0) throw(): mData(_mm_setr_ps(x, y, z, w))
		{
		}

		Vec(__m128 _data) throw(): mData(_data)
		{
		}

		Vec operator+(const Vec &v) const throw()
		{
			return _mm_add_ps(mData, v.mData);
		}

		Vec &operator+=(const Vec &v) throw()
		{
			mData = _mm_add_ps(mData, v.mData);
			return *this;
		}

        Vec operator-() const throw()
        {
            return _mm_sub_ps(_mm_setzero_ps(), mData);
        }

		Vec operator-(const Vec &v) const throw()
		{
			return _mm_sub_ps(mData, v.mData);
		}

		Vec &operator-=(const Vec &v) throw()
		{
			mData = _mm_sub_ps(mData, v.mData);
			return *this;
		}

		Vec operator*(float f) const throw()
		{
			return _mm_mul_ps(mData, _mm_load1_ps(&f));
		}

		Vec &operator*=(float f) throw()
		{
			mData = _mm_mul_ps(mData, _mm_load1_ps(&f));
			return *this;
		}

		Vec operator/(float f) const throw()
		{
			return _mm_div_ps(mData, _mm_load1_ps(&f));
		}

		Vec &operator/=(float f) throw()
		{
			mData = _mm_div_ps(mData, _mm_load1_ps(&f));
			return *this;
		}

		float operator[](size_t i) const throw(std::out_of_range)
		{
			if(i & ~3U)
				throw std::out_of_range("Vec::operator[]");
			float val[4];
            _mm_storer_ps(val, mData);
            return val[i];
		}

		float dot(const Vec &v) const throw()
		{
			return dot4(v);
		}

		float dot4(const Vec &v) const throw()
		{
			return _mm_cvtss_f32(_mm_dp_ps(mData, v.mData, 0xF1));
		}

		float dot3(const Vec &v) const throw()
		{
			return _mm_cvtss_f32(_mm_dp_ps(mData, v.mData, 0xE1));
		}

		float dot2(const Vec &v) const throw()
		{
			return _mm_cvtss_f32(_mm_dp_ps(mData, v.mData, 0xB1));
		}

 		Vec cross(Vec v) const throw()
		{
			__m128 a1 = _mm_setr_ps((*this)[1], (*this)[2], (*this)[2], (*this)[0]);
			__m128 a2 = _mm_setr_ps((*this)[0], (*this)[1], 0, 0);
			__m128 b1 = _mm_setr_ps(v[2], v[1], v[0], v[2]);
			__m128 b2 = _mm_setr_ps(v[1], v[0], 0, 0);
			return _mm_hsub_ps(_mm_mul_ps(a1, b1), _mm_mul_ps(a2, b2));
		}

		float length() const throw()
		{
			return std::sqrt(dot4(*this));
		}

		float length2() const throw()
		{
		    return dot4(*this);
		}

		void normalize() throw()
		{
			mData = _mm_mul_ps(mData, _mm_rsqrt_ps(_mm_dp_ps(mData, mData, 0xFF)));
		}

		__m128 data() const throw()
		{
		    return mData;
		}
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
