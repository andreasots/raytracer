#include "raytracer/tri.h"

#include "raytracer/allocator.h"

#include <cassert>
#include <limits>

#include <iostream>

inline bool equal(const RT_FLOAT &a, const RT_FLOAT &b)
{
    return std::abs(a-b) <= 20*std::numeric_limits<RT_FLOAT>::epsilon()*std::max<RT_FLOAT>(1, std::max(std::abs(a), std::abs(b)));
}

namespace Raytracer {

Tri::Tri(const SIMD::Point& p1, const SIMD::Point& p2,
         const SIMD::Point& p3, Material *mat): Object(mat)
{
#ifdef __SSE4_1__
    SIMD::Vec AB = p2-p1, AC = p3-p1;

    this->n = AB.cross(AC);
    RT_FLOAT denom = this->n.dot(this->n);

    this->n1 = AC.cross(this->n)/denom;
    this->n2 = this->n.cross(AB)/denom;

    const RT_FLOAT d  = this->n.dot (SIMD::Vec(p1.data()));
    const RT_FLOAT d1 = -this->n1.dot(SIMD::Vec(p1.data()));
    const RT_FLOAT d2 = -this->n2.dot(SIMD::Vec(p1.data()));

    this->n  += SIMD::Vec(0, 0, 0, d);
    this->n1 += SIMD::Vec(0, 0, 0, d1);
    this->n2 += SIMD::Vec(0, 0, 0, d2);

    this->n = SIMD::Vec(_mm_shuffle_ps(n.data(), n.data(), 0x1B));
    this->n1 = SIMD::Vec(_mm_shuffle_ps(n1.data(), n1.data(), 0x1B));
    this->n2 = SIMD::Vec(_mm_shuffle_ps(n2.data(), n2.data(), 0x1B));

    this->m_normals[0] = SIMD::Vec(0, 0, 0);
#else
    SIMD::Vec e0 = p2-p1, e1 = p3-p1, n = e0.cross(e1);
    RT_FLOAT nw = -1;
    int w;

    m_normals[0] = n;
    m_normals[0].normalize();

    for(size_t i = 0; i < 3U; i++)
        if(std::abs(n[i]) > nw)
        {
            nw = std::abs(n[i]);
            w = i;
        }
    nw = n[w];
    int u = (w == 0? 1: 0);
    int v = (w == 2? 1: 2);

    nw = 1/nw;
    nu = n[u]*nw;
    nv = n[v]*nw;
    pu = p1[u];
    pv = p1[v];
    np = nu*pu+nv*pv+p1[w];

    nw = (w == 1? -1: 1)*nw;
    e0u = e0[u]*nw;
    e0v = e0[v]*nw;
    e1u = e1[u]*nw;
    e1v = e1[v]*nw;

    ci = w;
    if(equal(nu, 0) && equal(nv, 0))
        ci |= 8;
#endif
}

Tri::~Tri()
{
    //dtor
}

SIMD::Matrix Tri::tangentSpace(RT_FLOAT u, RT_FLOAT v) const
{
    SIMD::Vec N;
#ifdef __SSE4_1__
    if(m_normals[0].dot(m_normals[0]) != 0)
#else
    if(ci & 16)
#endif
    {
        RT_FLOAT w = 1 - u - v;
        N = m_normals[0]*w+m_normals[1]*u+m_normals[2]*v;
        N.normalize();
    }
    else
#ifdef __SSE4_1__
    {
        N = SIMD::Vec(this->n[3], this->n[2], this->n[1]);
        N.normalize();
    }
#else
        N = m_normals[0];
#endif

    SIMD::Vec U, V;
    if(equal(N[0], 0))
        U = SIMD::Vec(1, 0, 0).cross(N);
    else
        U = SIMD::Vec(0, 1, 0).cross(N);
    U.normalize();
    V = N.cross(U);

    return SIMD::Matrix(U.data(), V.data(), N.data(), SIMD::Point().data());
}

RT_FLOAT Tri::intersect(const SIMD::Ray &r, RT_FLOAT &_u, RT_FLOAT &_v) const
{
#ifdef __SSE4_1__
    const __m128 o = _mm_shuffle_ps(r.origin.data(), r.origin.data(), 0x1B);
    const __m128 d = _mm_shuffle_ps(r.direction.data(), r.direction.data(), 0x1B);

    const __m128 det = _mm_dp_ps(n.data(), d, 0x7f);
    const __m128 dett = _mm_dp_ps(_mm_mul_ps(_mm_set_ps(1, -1, -1, -1), n.data()), o, 0xff);
    const __m128 oldt = _mm_set_ss(HUGE_VAL);

    if((_mm_movemask_ps(_mm_xor_ps(dett, _mm_sub_ss(_mm_mul_ss(oldt, det), dett)))&1) == 0)
    {
        const __m128 detp = _mm_add_ps(_mm_mul_ps(o, det), _mm_mul_ps(dett, d));
        const __m128 detu = _mm_dp_ps(detp, n1.data(), 0xf1);

        if((_mm_movemask_ps(_mm_xor_ps(detu, _mm_sub_ss(det, detu)))&1) == 0)
        {
            const __m128 detv = _mm_dp_ps(detp, n2.data(), 0xf1);

            if((_mm_movemask_ps(_mm_xor_ps(detv, _mm_sub_ss(det, _mm_add_ss(detu, detv))))&1) == 0)
            {
                __m128 inv_det = _mm_rcp_ss(det);
                inv_det = _mm_mul_ss(inv_det, _mm_sub_ss(_mm_set_ss(2), _mm_mul_ss(det, inv_det)));
                _u = _mm_cvtss_f32(_mm_mul_ss(detu, inv_det));
                _v = _mm_cvtss_f32(_mm_mul_ss(detv, inv_det));
                RT_FLOAT t = _mm_cvtss_f32(_mm_mul_ss(dett, inv_det));
                if(equal(t, 0))
                    t = HUGE_VAL;
                return t;
            }
        }
    }
    return HUGE_VAL;
#else
    const int w = ci & 3;
    const int u = (w == 0? 1: 0);
    const int v = (w == 2? 1: 2);
    const SIMD::Point o = r.origin;
    const SIMD::Vec d = r.direction;

    RT_FLOAT det, dett;
    // AA case
    if(ci&8)
    {
        det = d[w];
        dett = np-o[w];
    }
    else
    {
        det = d[u]*nu+d[v]*nv+d[w];
        dett = np-o[u]*nu-o[v]*nv-o[w];
    }
    const RT_FLOAT Du = d[u]*dett-(pu-o[u])*det;
    const RT_FLOAT Dv = d[v]*dett-(pv-o[v])*det;
    const RT_FLOAT detu = e1v*Du-e1u*Dv;
    const RT_FLOAT detv = e0u*Dv-e0v*Du;

    const RT_FLOAT tmpdet = det - detu - detv;
    RT_UINT tmpdet0 = reinterpret_cast<const RT_UINT&>(tmpdet) ^ reinterpret_cast<const RT_UINT&>(detu);
    RT_UINT tmpdet1 = reinterpret_cast<const RT_UINT&>(detv) ^ reinterpret_cast<const RT_UINT&>(detu);
    tmpdet0 = tmpdet0 | tmpdet1;
    if(tmpdet0 & RT_MSB)
        return HUGE_VAL;

    det = 1/det;
    const RT_FLOAT t = dett*det;
    if(t < 0 || equal(t, 0) || t == HUGE_VAL || isnan(t))
        return HUGE_VAL;
    _u = detu*det;
    if(_u < 0)
        return HUGE_VAL;
    _v = detv*det;
    if(_v < 0 || _u+_v > 1)
        return HUGE_VAL;
    return t;
#endif
}

void Tri::normals(const SIMD::Vec &n1, const SIMD::Vec &n2, const SIMD::Vec &n3)
{
    m_normals[0] = n1;
    m_normals[1] = n2;
    m_normals[2] = n3;
#ifndef __SSE4_1__
    ci |= 16;
#endif
}

} // namespace Raytracer
