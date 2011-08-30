#include "raytracer/sphere.h"

#include <gmtl/Containment.h>
#include <gmtl/Intersection.h>

namespace Raytracer {

Sphere::Sphere(gmtl::Point<RT_FLOAT, 3> o, RT_FLOAT r, Material mat): gmtl::Sphere<RT_FLOAT>(o, r), Object(mat)
{
    //ctor
}

Sphere::~Sphere()
{
    //dtor
}

const Material &Sphere::material(RT_FLOAT u, RT_FLOAT v)
{
    // Texture coordinates:
    // RT_FLOAT cos_theta = gmtl::dot(z, n);
    // RT_FLOAT cos_phi = gmtl::dot(x, n);

    return m_mat;
}

gmtl::Vec<RT_FLOAT, 3> Sphere::normal(RT_FLOAT u, RT_FLOAT v)
{
    gmtl::Vec<RT_FLOAT, 3> n(getRadius()*std::cos(u)*std::sin(v), getRadius()*std::sin(u)*std::sin(v), getRadius()*std::cos(v));
    gmtl::normalize(n);
    return n;
}

RT_FLOAT Sphere::intersect(const gmtl::Ray<RT_FLOAT> &r, RT_FLOAT &u, RT_FLOAT &v)
{
    int hits;
    RT_FLOAT t0, t1, t = HUGE_VAL;
    if(gmtl::intersect(*this, r, hits, t0, t1))
    {
        if(t0 > 0)
            t = t0;
        else if(t1 > 0 && t1 < t)
            t = t1;
        gmtl::Point<RT_FLOAT, 3> p = r.getOrigin()+t*r.getDir()-getCenter();
        u = std::atan(p[1]/p[0]);
        v = std::acos(p[2]/getRadius());
    }
    return t;
}

gmtl::AABox<RT_FLOAT> Sphere::bounds()
{
    gmtl::AABox<RT_FLOAT> box;
    gmtl::makeVolume(box, *this);
    return box;
}

} // namespace Raytracer
