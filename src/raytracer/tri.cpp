#include "raytracer/tri.h"

#include <gmtl/TriOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Intersection.h>
#include <gmtl/Output.h>

#include <cassert>
#include <limits>

inline bool equal(const RT_FLOAT &a, const RT_FLOAT &b)
{
    return std::abs(a-b) <= std::numeric_limits<RT_FLOAT>::epsilon()*std::max<RT_FLOAT>(1, std::max(std::abs(a), std::abs(b)));
}

namespace Raytracer {

Tri::Tri(const gmtl::Point<RT_FLOAT, 3>& p1, const gmtl::Point<RT_FLOAT, 3>& p2,
         const gmtl::Point<RT_FLOAT, 3>& p3, Material mat): Object(mat), m_hasNormals(false)
{
    gmtl::Vec<RT_FLOAT, 3> e0 = p2-p1, e1 = p3-p1, n;
    gmtl::cross(n, e0, e1);
    RT_FLOAT nw = -1;
    int w;
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

    gmtl::Point<RT_FLOAT, 3> min = p1;
    gmtl::Point<RT_FLOAT, 3> max = p1;
    for(size_t j = 0; j < 3; j++)
    {
        if(min[j] > p2[j])
            min[j] = p2[j];
        if(max[j] < p2[j])
            max[j] = p2[j];
    }
    for(size_t j = 0; j < 3; j++)
    {
        if(min[j] > p3[j])
            min[j] = p3[j];
        if(max[j] < p3[j])
            max[j] = p3[j];
    }
    m_bbox = new gmtl::AABox<RT_FLOAT>(min, max);

}

Tri::~Tri()
{
    //dtor
    delete m_bbox;
}

const Material &Tri::material(RT_FLOAT u, RT_FLOAT v)
{
    return m_mat;
}

gmtl::Vec<RT_FLOAT, 3> Tri::normal(RT_FLOAT u, RT_FLOAT v)
{
    gmtl::Vec<RT_FLOAT, 3> ret;
    if(!m_hasNormals)
    {
        ret[ci&3] = 1;
        ret[ci&3 == 0? 1: 0] = nu;
        ret[ci&3 == 2? 1: 2] = nv;
    }
    else
    {
        RT_FLOAT w = 1 - u - v;
        ret = m_normals[0]*w+m_normals[1]*u+m_normals[2]*v;
    }
    gmtl::normalize(ret);
    return ret;
}

RT_FLOAT Tri::intersect(const gmtl::Ray<RT_FLOAT> &r, RT_FLOAT &_u, RT_FLOAT &_v)
{
    const int w = ci & 3;
    const int u = (w == 0? 1: 0);
    const int v = (w == 2? 1: 2);
    const gmtl::Point<RT_FLOAT, 3> o = r.getOrigin();
    const gmtl::Vec<RT_FLOAT, 3> d = r.getDir();

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

gmtl::AABox<RT_FLOAT> Tri::bounds()
{
    return *m_bbox;
}


void Tri::normals(const gmtl::Vec<RT_FLOAT, 3> &n1, const gmtl::Vec<RT_FLOAT, 3> &n2, const gmtl::Vec<RT_FLOAT, 3> &n3)
{
    m_hasNormals = true;
    m_normals[0] = n1;
    m_normals[1] = n2;
    m_normals[2] = n3;
}

} // namespace Raytracer
