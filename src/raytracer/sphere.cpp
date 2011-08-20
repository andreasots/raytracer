#include "raytracer/sphere.h"

#include <gmtl/Containment.h>
#include <gmtl/Intersection.h>

namespace Raytracer {

Sphere::Sphere(gmtl::Point<FLOAT, 3> o, FLOAT r, Material mat): gmtl::Sphere<FLOAT>(o, r), Object(mat)
{
    //ctor
}

Sphere::~Sphere()
{
    //dtor
}

const Material &Sphere::material(const gmtl::Point<FLOAT, 3> &p)
{
    // Texture coordinates:
    // FLOAT cos_theta = gmtl::dot(z, n);
    // FLOAT cos_phi = gmtl::dot(x, n);

    return m_mat;
}

gmtl::Vec<FLOAT, 3> Sphere::normal(const gmtl::Point<FLOAT, 3> &p)
{
    gmtl::Vec<FLOAT, 3> n = p - this->getCenter();
    gmtl::normalize(n);
    return n;
}

FLOAT Sphere::intersect(const gmtl::Ray<FLOAT> &r)
{
    int hits;
    FLOAT t0, t1;
    if(gmtl::intersect(*this, r, hits, t0, t1))
    {
        if(t0 < t1 && t0 > 0)
            return t0;
        else if(t1 > 0)
            return t1;
    }
    return HUGE_VAL;
}

gmtl::AABox<FLOAT> Sphere::bounds()
{
    gmtl::AABox<FLOAT> box;
    gmtl::makeVolume(box, *this);
    return box;
}

gmtl::Point<FLOAT, 3> Sphere::random()
{
    FLOAT theta = 2*M_PI*drand48();
    FLOAT cos_theta = std::cos(theta), sin_theta = std::sin(theta);
    FLOAT cos_phi = drand48()*2-1;
    FLOAT sin_phi = std::sqrt(1-cos_phi*cos_phi);
    return getCenter() + gmtl::Vec<FLOAT, 3>(getRadius()*sin_phi*cos_theta, getRadius()*sin_phi*sin_theta, getRadius()*cos_phi);
}

} // namespace Raytracer
