#include "raytracer/cylinder.h"

namespace Raytracer {

Cylinder::Cylinder(SIMD::Point A, SIMD::Point B, RT_FLOAT r, Material *mat): m_A(A), m_a(B-A), Object(mat)
{
    m_d = std::copysign(r*r*m_a.dot(m_a), r);
}

Cylinder::~Cylinder()
{
}

SIMD::Matrix Cylinder::tangentSpace(RT_FLOAT _u, RT_FLOAT _v) const
{
    SIMD::Vec i, j, u = m_a;
    u.normalize();
    if(std::abs(u[0]) > 0.1)
        i = SIMD::Vec(0, 1, 0).cross(u);
    else
        i = SIMD::Vec(1, 0, 0).cross(u);
    i.normalize();
    j = u.cross(i);
    SIMD::Vec n = i*std::sin(_v*2*M_PI)+j*std::cos(_v*2*M_PI);
    n *= std::copysign(1, m_d);

    SIMD::Vec v = n.cross(u);

    return SIMD::Matrix(u.data(), v.data(), n.data(), SIMD::Point().data());
}

RT_FLOAT Cylinder::intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v) const
{
    static const RT_FLOAT eps = 1e-4*std::abs(m_d);
    SIMD::Vec V = (r.origin-m_A).cross(m_a), U = r.direction.cross(m_a);

    RT_FLOAT a = U.dot(U);
    RT_FLOAT b = V.dot(U);
    RT_FLOAT c = V.dot(V)-std::abs(m_d);
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
    u = (P-m_A).dot(m_a);
    RT_FLOAT a2 = m_a.dot(m_a);

    if((u < 0 || u > a2) && (t2 < HUGE_VAL))
    {
        t = t2;
        P = r.origin+t*r.direction;
        u = (P-m_A).dot(m_a);
    }
    u /= a2;

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

} // namespace Raytracer
