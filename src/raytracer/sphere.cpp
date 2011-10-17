#include "raytracer/sphere.h"
#include <cmath>

namespace Raytracer {

Sphere::Sphere(SIMD::Point o, RT_FLOAT r, Material mat): mO(o), mR(r), Object(mat)
{
    //ctor
}

Sphere::~Sphere()
{
    //dtor
}

const Material &Sphere::material(RT_FLOAT u, RT_FLOAT v)
{
    return m_mat;
}

SIMD::Vec Sphere::normal(RT_FLOAT u, RT_FLOAT v)
{
    RT_FLOAT phi = v*M_PI;
    RT_FLOAT theta = u*2*M_PI;
    SIMD::Vec n(std::cos(theta)*std::sin(phi), std::sin(theta)*std::sin(phi), std::cos(phi));
    return n;
}

RT_FLOAT Sphere::intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v)
{
    RT_FLOAT p = r.direction.dot(r.origin-mO);
    RT_FLOAT det = p*p-(r.origin-mO).length2()+mR*mR;
    if(det < 0)
        return HUGE_VAL;
    det = std::sqrt(det);
    RT_FLOAT t = std::min((p+det) < 0 ? HUGE_VAL: p+det, (p-det) < 0? HUGE_VAL: p-det);

    // Ray Tracing: Graphics for the Masses
    // by Paul Rademacher
    // http://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html

    SIMD::Vec Vp = r.origin+t*r.direction - mO;
    Vp.normalize();
    SIMD::Vec Vn(0, 0, 1), Ve(1, 0, 0);

    v = std::acos(-Vn.dot(Vp));
    u = (std::acos(Vp.dot(Ve))/std::sin(v))*0.5*M_1_PI;
    v *= M_1_PI;
    if(Vp.dot(Vn.cross(Ve)) < 0)
        u = 1 - u;

    return t;
}

SIMD::AABox Sphere::bounds()
{
    SIMD::Vec v(mR, mR, mR);
    return SIMD::AABox(mO-v, mO+v);
}

} // namespace Raytracer
