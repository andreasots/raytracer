#include "raytracer/object.h"
#include "raytracer/vector.h"
#include <cmath>

namespace Raytracer {

Object::Object(Vector<> pos, Material mat): m_pos(pos), m_mat(mat)
{
    //ctor
}

float Object::intersect(Ray<> &r)
{
    return HUGE_VALF;
}

Vector<> Object::normal(Vector<> pos)
{
    Vector<> ret;
    return ret;
}

} // namespace Raytracer
