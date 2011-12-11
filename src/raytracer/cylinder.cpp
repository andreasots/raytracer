#include "raytracer/cylinder.h"

namespace Raytracer {

Cylinder::Cylinder(SIMD::Point A, SIMD::Point B, RT_FLOAT r, Material mat): m_A(A), m_a(B-A), Object(mat)
{
    m_d = std::copysign(r*r*m_a.dot(m_a), r);
}

Cylinder::~Cylinder()
{
}

const Material &Cylinder::material(RT_FLOAT u, RT_FLOAT v)
{
    return m_mat;
}

SIMD::Vec Cylinder::normal(RT_FLOAT u, RT_FLOAT v)
{
    SIMD::Vec i, j, k = m_a;
    k.normalize();
    if(std::abs(k[0]) > 0.1)
        i = SIMD::Vec(0, 1, 0).cross(k);
    else
        i = SIMD::Vec(1, 0, 0).cross(k);
    i.normalize();
    j = k.cross(i);
    SIMD::Vec n = i*std::sin(v*2*M_PI)+j*std::cos(v*2*M_PI);
    n *= std::copysign(1, m_d);
    return n;
}

RT_FLOAT Cylinder::intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v)
{
    static const RT_FLOAT eps = 1e-4*std::abs(m_d);
    SIMD::Vec V = (r.origin-m_A).cross(m_a), U = r.direction.cross(m_a);

    RT_FLOAT a = U.dot(U);
    RT_FLOAT b = V.dot(U);
    RT_FLOAT c = V.dot(V)-std::copysign(m_d, 1);
    RT_FLOAT det = b*b-a*c;
    if(det < 0)
        return HUGE_VAL;
    RT_FLOAT q = -(b+std::copysign(std::sqrt(det), b));
    RT_FLOAT t = q/a, t2 = c/q;

    t = (t > eps ? t: HUGE_VAL);
    t2 = (t2 > eps ? t2: HUGE_VAL);
    if(t > t2)
    {
        auto tmp = t2;
        t2 = t;
        t = tmp;
    }

    if(t == HUGE_VAL && t2 == HUGE_VAL)
        return HUGE_VAL;

    SIMD::Point P = r.origin+t*r.direction;
    u = (P-m_A).dot(m_a)/m_a.dot(m_a);
    if(u < 0 || u > 1)
    {
        t = t2;
        P = r.origin+t*r.direction;
        u = (P-m_A).dot(m_a)/m_a.dot(m_a);
    }

    if(u < 0 || u > 1)
        return HUGE_VAL;

    SIMD::Vec i, k = m_a, R = P-(m_A+m_a*u);
    k.normalize();
    if(std::abs(k[0]) > 0.1)
        i = SIMD::Vec(0, 1, 0).cross(k);
    else
        i = SIMD::Vec(1, 0, 0).cross(k);
    i.normalize();
    SIMD::Vec j = k.cross(i);

    v = std::atan2(i.dot(R), j.dot(R))/(2*M_PI);
    if(v < 0)
        v += 1;
    return t;
}

SIMD::AABox Cylinder::bounds()
{
    RT_FLOAT m_r = std::sqrt(m_d/m_a.dot(m_a));
    const SIMD::Point A = m_A, B = m_A + m_a;
    SIMD::Vec n = m_a;
    n.normalize();
    const SIMD::Vec r(m_r*std::sqrt(1-n[0]*n[0]), m_r*std::sqrt(1-n[1]*n[1]),
                      m_r*std::sqrt(1-n[2]*n[2]));

    return SIMD::AABox(SIMD::Point(std::min(A[0], B[0]), std::min(A[1], B[1]),
                                   std::min(A[2], B[2])) - r,
                       SIMD::Point(std::max(A[0], B[0]), std::max(A[1], B[1]),
                                   std::max(A[2], B[2])) + r);
}

} // namespace Raytracer