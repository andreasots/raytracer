#include "raytracer/tri.h"

#include "raytracer/allocator.h"

#include <cassert>
#include <limits>

#include <iostream>

inline bool equal(const RT_FLOAT &a, const RT_FLOAT &b)
{
    return std::abs(a-b) <= 10*std::numeric_limits<RT_FLOAT>::epsilon()*std::max<RT_FLOAT>(1, std::max(std::abs(a), std::abs(b)));
}

namespace Raytracer {

Tri::Tri(const SIMD::Point& p1, const SIMD::Point& p2,
         const SIMD::Point& p3, Material *mat): Object(mat)
{
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
    m_bbox = new (allocate<SIMD::AABox, 16>(1)) SIMD::AABox(p1, p1);
    m_bbox->extend(SIMD::AABox(p2, p2));
    m_bbox->extend(SIMD::AABox(p3, p3));
}

Tri::~Tri()
{
    //dtor
    deallocate<SIMD::AABox, 16>(m_bbox, 1);
}

SIMD::Matrix Tri::tangentSpace(RT_FLOAT u, RT_FLOAT v)
{
    SIMD::Vec n;
    if(ci & 16)
    {
        RT_FLOAT w = 1 - u - v;
        n = m_normals[0]*w+m_normals[1]*u+m_normals[2]*v;
        n.normalize();
    }
    else
        n = m_normals[0];

    SIMD::Vec U, V;
    if(std::abs(n[0]) > 0.1)
        U = SIMD::Vec(0, 1, 0).cross(n);
    else
        U = SIMD::Vec(1, 0, 0).cross(n);
    U.normalize();
    V = n.cross(U);

    return SIMD::Matrix(U.data(), V.data(), n.data(), SIMD::Point().data());
}

RT_FLOAT Tri::intersect(const SIMD::Ray &r, RT_FLOAT &_u, RT_FLOAT &_v)
{
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

    // Russian bit magic
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
}
SIMD::AABox Tri::bounds()
{
    return *m_bbox;
}


void Tri::normals(const SIMD::Vec &n1, const SIMD::Vec &n2, const SIMD::Vec &n3)
{
    ci |= 16;
    m_normals[0] = n1;
    m_normals[1] = n2;
    m_normals[2] = n3;
}

} // namespace Raytracer
