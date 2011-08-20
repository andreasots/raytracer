#include "raytracer/tri.h"

#include <gmtl/TriOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Intersection.h>

#include <cassert>

#define ku modulo[m_k+1]
#define kv modulo[m_k+2]

inline void UV(const Raytracer::Tri &t, const gmtl::Point<FLOAT, 3> p, FLOAT &u, FLOAT &v)
{
    // Compute vectors
    gmtl::Vec<FLOAT, 3> v0 = t[2] - t[0], v1 = t[1] - t[0], v2 = p - t[0];

    // Compute dot products
    FLOAT dot00 = gmtl::dot(v0, v0), dot01 = gmtl::dot(v0, v1), dot02 = gmtl::dot(v0, v2);
    FLOAT dot11 = gmtl::dot(v1, v1), dot12 = gmtl::dot(v1, v2);

    // Compute barycentric coordinates
    FLOAT invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    v = (dot00 * dot12 - dot01 * dot02) * invDenom;
}

namespace Raytracer {

Tri::Tri(const gmtl::Point<FLOAT, 3>& p1, const gmtl::Point<FLOAT, 3>& p2,
         const gmtl::Point<FLOAT, 3>& p3, Material mat): gmtl::Tri<FLOAT>(p1, p2, p3), Object(mat), m_hasNormals(false)
{
/*    gmtl::Vec<FLOAT, 3> N;
    N = gmtl::normal(*this);
    //ctor
    char modulo[] = {0,1,2,0,1};
    if(std::abs(N[0]) > std::abs(N[1]))
        if(std::abs(N[0]) > abs(N[2]))
            m_k = 0;
        else
            m_k = 2;
    else
        if(std::abs(N[1]) > abs(N[2]))
            m_k = 1;
        else
            m_k = 2;
    m_n_u = N[ku] / N[m_k];
    m_n_v = N[kv] / N[m_k];
    m_n_d = gmtl::dot(N, mVerts[0])/ N[m_k];

    gmtl::Vec<FLOAT, 3> b = mVerts[2] - mVerts[0];
    gmtl::Vec<FLOAT, 3> c = mVerts[1] - mVerts[0];

    FLOAT invDet = (b[ku] * c[kv] - b[kv] * c[ku]);

    m_b_nu = b[ku] * invDet;
    m_b_nv = -b[kv] * invDet;
    m_c_nu = c[kv] * invDet;
    m_c_nv = -c[ku] * invDet;*/
}

Tri::~Tri()
{
    //dtor
}

const Material &Tri::material(const gmtl::Point<FLOAT, 3> &p)
{
    return m_mat;
}

gmtl::Vec<FLOAT, 3> Tri::normal(const gmtl::Point<FLOAT, 3> &p)
{
    if(!m_hasNormals)
        return gmtl::normal(*this);
    else
    {
        FLOAT u, v, w;
        UV(*this, p, u, v);
        w = 1 - u - v;
        gmtl::Vec<FLOAT, 3> ret = m_normals[0]*w+m_normals[1]*v+m_normals[2]*u;
        gmtl::normalize(ret);
        return ret;
    }
}

// Projected triangle intersection test by Ingo Wald
// Modifications by Jacco Bikker
FLOAT Tri::intersect(const gmtl::Ray<FLOAT> &r)
{
/*    char modulo[] = {0,1,2,0,1};
    FLOAT nd = 1./(r.getDir()[m_k] + m_n_u * r.getDir()[ku] + m_n_v * r.getDir()[kv]);
    FLOAT f = (m_n_d - r.getOrigin()[m_k] - m_n_u * r.getOrigin()[ku] - m_n_v * r.getOrigin()[kv]) * nd;

    if(f < 0)
        return HUGE_VAL;

    FLOAT hu = r.getOrigin()[ku] + f * r.getDir()[ku] - mVerts[0][ku];
    FLOAT hv = r.getOrigin()[kv] + f * r.getDir()[kv] - mVerts[0][ku];

    FLOAT lambda = hv * m_b_nu + hu * m_b_nv;
    if(lambda < 0)
        return HUGE_VAL;

    FLOAT mue = hu * m_c_nu + hv * m_c_nv;
    if(mue < 0)
        return HUGE_VAL;
    if(lambda+mue > 1)
        return HUGE_VAL;
    return f;*/
    float t, u, v;
    if(gmtl::intersect(*this, r, u, v, t))
        if(t > 0)
            return t;
    return HUGE_VAL;
}

gmtl::AABox<FLOAT> Tri::bounds()
{
    gmtl::Point<FLOAT, 3> min(HUGE_VAL, HUGE_VAL, HUGE_VAL);
    gmtl::Point<FLOAT, 3> max(-HUGE_VAL, -HUGE_VAL, -HUGE_VAL);

    for(size_t i = 0; i < 3; i++)
        for(size_t j = 0; j < 3; j++)
        {
            if(min[j] > mVerts[i][j])
                min[j] = mVerts[i][j];
            if(max[j] < mVerts[i][j])
                max[j] = mVerts[i][j];
        }

    return gmtl::AABox<FLOAT>(min, max);
}

gmtl::Point<FLOAT, 3> Tri::random()
{
    gmtl::Vec<FLOAT, 3> v1 = mVerts[1] - mVerts[0], v2 = mVerts[2] - mVerts[0];
    FLOAT a1, a2;
    do
    {
        a1 = drand48();
        a2 = drand48();
    }
    while(a1+a2 > 1);

    return mVerts[0] + v1*a1 + v2*a2;
}

void Tri::normals(const gmtl::Vec<FLOAT, 3> &n1, const gmtl::Vec<FLOAT, 3> &n2, const gmtl::Vec<FLOAT, 3> &n3)
{
    m_hasNormals = true;
    m_normals[0] = n1;
    m_normals[1] = n2;
    m_normals[2] = n3;
}

} // namespace Raytracer
