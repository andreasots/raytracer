#include "raytracer/sphere.h"

#include <algorithm>

namespace Raytracer {

Sphere::Sphere(Vector<> pos, Material mat, float r): Object(pos, mat), m_r2(r*r)
{
    //ctor
}

float Sphere::intersect(Ray<> &r)
{
    Vector<> v = m_pos - r.origin();
    float p = -r.direction().dot(v), det = sqrt(p*p - v.dot(v) - this->m_r2);

    return std::min(p - det, p + det);
}

Vector<> Sphere::normal(Vector<> pos)
{
    Vector<> ret = pos-this->m_pos;
    ret = ret.normalise();
    return ret;
}

} // namespace Raytracer
