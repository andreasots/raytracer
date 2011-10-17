#ifndef SIMDPOINT_H_INCLUDED
#define SIMDPOINT_H_INCLUDED
#include <xmmintrin.h>
#include "Vec.h"
namespace SIMD
{
class Point
{
    __m128 mData;
    public:
        Point() throw(): mData(_mm_set_ps(0,0,0,1))
        {
        }

        Point(float x, float y, float z, float w = 1) throw(): mData(_mm_set_ps(x, y, z, w))
        {
        }

        Point(__m128 _data) throw(): mData(_data)
        {
        }

        Vec operator-(const Point& p) const throw()
        {
            return _mm_sub_ps(mData, p.mData);
        }

        Point operator+(const Vec& v) const throw()
        {
            return _mm_add_ps(mData, v.data());
        }

        Point operator-(const Vec& v) const throw()
        {
            return _mm_sub_ps(mData, v.data());
        }

        Point &operator+=(const Vec &v) throw()
        {
            mData = _mm_add_ps(mData, v.data());
            return *this;
        }

        float operator[](size_t i) const throw(std::out_of_range)
        {
            if(i & ~3U)
                throw std::out_of_range("Point::operator[]");
            float val[4];
            _mm_storer_ps(val, mData);
            return val[i];
        }

        __m128 data() const throw()
        {
            return mData;
        }
};
inline std::ostream &operator<<(std::ostream &out, const Point &v)
{
	out << '(' << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ')';
	return out;
}
}
#endif // SIMDPOINT_H_INCLUDED
