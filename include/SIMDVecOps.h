#ifndef SIMDVECOPS_H_INCLUDED
#define SIMDVECOPS_H_INCLUDED

#include <xmmintrin.h>
#include <gmtl/Vec.h>

inline float SIMDDot(const gmtl::Vec<float, 3> &_a, const gmtl::Vec<float, 3> &_b)
{
    __m128 a = {_a[0], _a[1], _a[2], 0}, b = {_b[0], _b[1], _b[2], 0};
    __m128 val = _mm_mul_ps(a, b);
    float *tmp, ret;
    posix_memalign(reinterpret_cast<void**>(&tmp), 16, sizeof(float)*4);
    ret = tmp[0] + tmp[1] + tmp[2] + tmp[3];
    free(tmp);
    return ret;
}

inline float* SIMDDot(const gmtl::Vec<float, 3> &a0, const gmtl::Vec<float, 3> &b0,
                      const gmtl::Vec<float, 3> &a1, const gmtl::Vec<float, 3> &b1,
                      const gmtl::Vec<float, 3> &a2, const gmtl::Vec<float, 3> &b2,
                      const gmtl::Vec<float, 3> &a3, const gmtl::Vec<float, 3> &b3)
{
    float *ret = new float[4];
    __m128 a, b, r0, r1;
    a = _mm_setr_ps(a0[0], a1[0], a2[0], a3[0]);
    b = _mm_setr_ps(b0[0], b1[0], b2[0], b3[0]);
    r0 = _mm_mul_ps(a, b);
    a = _mm_setr_ps(a0[1], a1[1], a2[1], a3[1]);
    b = _mm_setr_ps(b0[1], b1[1], b2[1], b3[1]);
    r1 = _mm_mul_ps(a, b);
    r0 = _mm_add_ps(r0, r1);
    a = _mm_setr_ps(a0[2], a1[2], a2[2], a3[2]);
    b = _mm_setr_ps(b0[2], b1[2], b2[2], b3[2]);
    _mm_storeu_ps(ret, _mm_add_ps(r0, _mm_mul_ps(a, b)));
    return ret;
}

inline float* SIMDLength(const gmtl::Vec<float, 3> &a,
                         const gmtl::Vec<float, 3> &b,
                         const gmtl::Vec<float, 3> &c,
                         const gmtl::Vec<float, 3> &d)
{
    float *squared = SIMDDot(a, a, b, b, c, c, d, d);
    _mm_storeu_ps(squared, _mm_sqrt_ps(_mm_loadu_ps(squared)));
    return squared;
}

inline float SIMDTripleProduct(const gmtl::Vec<float, 3> &a, const gmtl::Vec<float, 3> &b, const gmtl::Vec<float, 3> &c)
{
    float *tmp = new float[4], ret;
    __m128 r;
    r = _mm_mul_ps(_mm_setr_ps(a[0], a[1], a[2], a[2]), _mm_setr_ps(b[1], b[2], b[0], b[1]));
    r = _mm_mul_ps(r, _mm_setr_ps(c[2], c[0], c[1], c[0]));
    _mm_storeu_ps(tmp, r);
    ret = tmp[0] + tmp[1] + tmp[2] - tmp[3];
    r = _mm_mul_ps(_mm_setr_ps(a[0], a[1], 0, 0), _mm_setr_ps(b[2], b[0], 0, 0));
    r = _mm_mul_ps(r, _mm_setr_ps(c[1], c[2], 0, 0));
    _mm_storeu_ps(tmp, r);
    ret -= tmp[0] + tmp[1];
    delete[] tmp;
    return ret;
}
#endif // SIMDVECOPS_H_INCLUDED
