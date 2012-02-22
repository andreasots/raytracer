#include "raytracer/quad.h"

#include "raytracer/allocator.h"

#include <cassert>
#include <limits>

#include <iostream>

inline bool equal(const RT_FLOAT &a, const RT_FLOAT &b)
{
    return std::abs(a-b) <= 20*std::numeric_limits<RT_FLOAT>::epsilon()*std::max<RT_FLOAT>(1, std::max(std::abs(a), std::abs(b)));
}

namespace Raytracer {

Quad::Quad(const SIMD::Point& A, const SIMD::Point& B, const SIMD::Point& C, const SIMD::Point& D, Material *mat):
    Object(mat), v00(A), v10(B), v11(C), v01(D)
{
    SIMD::Vec e01 = v10 - v00;
    SIMD::Vec e02 = v11 - v00;
    SIMD::Vec e03 = v01 - v00;
    SIMD::Vec n = e01.cross(e03);

    m_normals[0] = n;
    m_normals[0].normalize();

    if((std::abs(n[0]) >= std::abs(n[1])) && (std::abs(n[0]) >= std::abs(n[2])))
    {
        a11 = (e02[1]*e03[2] - e02[2]*e03[1])/n[0];
        b11 = (e01[1]*e03[2] - e01[2]*e03[1])/n[0];
    }
    else if((std::abs(n[1]) >= std::abs(n[0])) && (std::abs(n[1]) >= std::abs(n[2])))
    {
        a11 = (e02[2]*e03[0] - e02[0]*e03[2])/n[1];
        b11 = (e01[2]*e03[0] - e01[0]*e03[2])/n[1];
    }
    else
    {
        a11 = (e02[0]*e03[1] - e02[1]*e03[0])/n[2];
        b11 = (e01[0]*e03[1] - e01[1]*e03[0])/n[2];
    }

    if(a11 < 0 || a11 > 1 || b11 < 0 || b11 > 1)
    {
        v00 = C;
        v10 = D;
        v11 = A;
        v01 = B;

        e01 = v10 - v00;
        e02 = v11 - v00;
        e03 = v01 - v00;
        n = e01.cross(e03);

        if((std::abs(n[0]) >= std::abs(n[1])) && (std::abs(n[0]) >= std::abs(n[2])))
        {
            a11 = (e02[1]*e03[2] - e02[2]*e03[1])/n[0];
            b11 = (e01[1]*e03[2] - e01[2]*e03[1])/n[0];
        }
        else if((std::abs(n[1]) >= std::abs(n[0])) && (std::abs(n[1]) >= std::abs(n[2])))
        {
            a11 = (e02[2]*e03[0] - e02[0]*e03[2])/n[1];
            b11 = (e01[2]*e03[0] - e01[0]*e03[2])/n[1];
        }
        else
        {
            a11 = (e02[0]*e03[1] - e02[1]*e03[0])/n[2];
            b11 = (e01[0]*e03[1] - e01[1]*e03[0])/n[2];
        }
    }
}

Quad::~Quad()
{
    //dtor
}

SIMD::Matrix Quad::tangentSpace(RT_FLOAT u, RT_FLOAT v) const
{
    SIMD::Vec N;
    if(flags & 1)
    {
        N  = (1-u)*(1-v)*m_normals[0];
        N += u*(1-v)*m_normals[1];
        N += u*v*m_normals[2];
        N += (1-u)*v*m_normals[3];
        N.normalize();
    }
    else
        N = m_normals[0];

    SIMD::Vec U, V;
    if(equal(N[0], 0))
        U = SIMD::Vec(1, 0, 0).cross(N);
    else
        U = SIMD::Vec(0, 1, 0).cross(N);
    U.normalize();
    V = N.cross(U);

    return SIMD::Matrix(U.data(), V.data(), N.data(), SIMD::Point().data());
}

RT_FLOAT Quad::intersect(const SIMD::Ray &r, RT_FLOAT &_u, RT_FLOAT &_v) const
{
    SIMD::Vec e01 = v10 - v00;
    SIMD::Vec e03 = v01 - v00;
    SIMD::Vec p = r.direction.cross(e03);
    RT_FLOAT det = e01.dot(p);
    if(equal(det, 0))
        return HUGE_VAL;
    det = 1/det;
    SIMD::Vec T = r.origin - v00;
    RT_FLOAT a = T.dot(p)*det;
    if(a < 0 || a > 1)
        return HUGE_VAL;
    SIMD::Vec q = T.cross(e01);
    RT_FLOAT b = r.direction.dot(q)*det;
    if(b < 0 || b > 1)
        return HUGE_VAL;

    if(a+b > 1)
    {
        SIMD::Vec e23 = v01 - v11;
        SIMD::Vec e21 = v10 - v11;
        SIMD::Vec p2 = r.direction.cross(e21);
        RT_FLOAT det2 = e23.dot(p2);
        if(equal(det2, 0))
            return HUGE_VAL;
        det2 = 1/det2;
        SIMD::Vec t =  r.origin - v11;
        RT_FLOAT a2 = t.dot(p2)*det2;
        if(a2 < 0)
            return HUGE_VAL;
        SIMD::Vec q2 = t.cross(e23);
        RT_FLOAT b2 = r.direction.dot(q2)*det;
        if(b2 < 0)
            return HUGE_VAL;
    }

    RT_FLOAT t = e03.dot(q)*det;
    if(t < 0 || equal(t, 0))
        return HUGE_VAL;

    if(equal(a11, 1))
    {
        _u = a;
        if(equal(b11, 1))
            _v = b;
        else
            _v = b/(_u*(b11-1)+1);
    }
    else if(equal(b11, 1))
    {
        _v = b;
        _u = a/(_v*(a11-1)+1);
    }
    else
    {
        RT_FLOAT A = -(b11-1);
        RT_FLOAT B = a*(b11-1)-b*(a11-1)-1;
        RT_FLOAT C = a;
        RT_FLOAT d = B*B-4*A*C;
        RT_FLOAT Q = -0.5*(B+std::copysign(std::sqrt(d), B));
        _u = Q/A;
        if(_u < 0 || _u > 1)
            _u = C/Q;
        _v = b/(_u*(b11-1)+1);
    }
    return t;
}

void Quad::normals(const SIMD::Vec &n1, const SIMD::Vec &n2, const SIMD::Vec &n3, const SIMD::Vec &n4)
{
    flags |= 1;
    m_normals[0] = n1;
    m_normals[1] = n2;
    m_normals[2] = n3;
    m_normals[3] = n4;
}

} // namespace Raytracer
