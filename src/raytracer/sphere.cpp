#include "raytracer/sphere.h"
#include <cmath>
#include <limits>

inline bool equal(const RT_FLOAT &a, const RT_FLOAT &b)
{
    return std::abs(a-b) <= 10*std::numeric_limits<RT_FLOAT>::epsilon()*std::max<RT_FLOAT>(1, std::max(std::abs(a), std::abs(b)));
}

namespace Raytracer {

Sphere::Sphere(SIMD::Point o, RT_FLOAT r, Material *mat): mO(o), mR(r), Object(mat)
{
    //ctor
}

Sphere::~Sphere()
{
    //dtor
}

SIMD::Vec Sphere::normal(RT_FLOAT u, RT_FLOAT v)
{
    RT_FLOAT phi = u*M_PI;
    RT_FLOAT theta = v*(2*M_PI);
    SIMD::Vec n(std::cos(theta)*std::sin(phi), std::sin(theta)*std::sin(phi), std::cos(phi));
    return n;
}

RT_FLOAT Sphere::intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v)
{
    static const RT_FLOAT eps = 1e-4*mR;
    SIMD::Vec V = r.origin - mO;
    RT_FLOAT b = r.direction.dot(V);
    RT_FLOAT c = V.dot(V) - mR*mR;
    RT_FLOAT det = b*b-c;
    if(det < 0)
        return HUGE_VAL;
    RT_FLOAT t1 = -(b+std::copysign(std::sqrt(det), b));
    RT_FLOAT t2 = c/t1;
    RT_FLOAT t = std::min((t1 > eps ? t1: HUGE_VAL), (t2 > eps? t2: HUGE_VAL));
    if(t == HUGE_VAL)
        return HUGE_VAL;

    V = r.origin+t*r.direction - mO;
    RT_FLOAT theta = std::acos(V[2]/mR);
    RT_FLOAT phi = std::atan2(V[1], V[0]);
    if(phi < 0)
        phi += 2*M_PI;
    u = theta/M_PI;
    v = phi/(2*M_PI);

    return t;
}

SIMD::AABox Sphere::bounds()
{
    SIMD::Vec v(mR, mR, mR);
    return SIMD::AABox(mO-v, mO+v);
}

} // namespace Raytracer
