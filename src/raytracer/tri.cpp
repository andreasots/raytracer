#include "raytracer/tri.h"

#include <gmtl/TriOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Intersection.h>

#include <cassert>

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
         const gmtl::Point<FLOAT, 3>& p3, Material mat)/*: Tri(p1, p2, p3), m_mat(mat)*/
{
    set(p1, p2, p3);
    m_mat = mat;
    m_hasNormals = false;
    //ctor
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

FLOAT Tri::intersect(const gmtl::Ray<FLOAT> &r)
{
    float t, u, v;
    if(gmtl::intersect(*this, r, u, v, t) && t > 0)
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
    FLOAT A = drand48(), B = drand48(), C;
    if(A+B <= 1)
        C = 1-A-B;
    else
    {
        C = drand48();
        FLOAT l = 1/(A+B+C);
        A *= l;
        B *= l;
        C *= l;
    }

    return mVerts[0]*A + mVerts[1]*B + mVerts[2]*C;
}

void Tri::normals(const gmtl::Vec<FLOAT, 3> &n1, const gmtl::Vec<FLOAT, 3> &n2, const gmtl::Vec<FLOAT, 3> &n3)
{
    m_hasNormals = true;
    m_normals[0] = n1;
    m_normals[1] = n2;
    m_normals[2] = n3;
}

} // namespace Raytracer
