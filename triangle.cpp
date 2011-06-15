#include "point.h"
#include "vector.h"
#include "triangle.h"
#include <cmath>

namespace Raytracer
{
    Triangle::Triangle(Point a,
                       Point b,
                       Point c) throw(): A(a), B(b), C(c), n(0, 0, 0)
    {
        n = (b-a)*(c-a);
        n /= sqrt(n*n);
    }
    
    Triangle::Triangle(Point a,
                       Point b,
                       Point c,
                       Vector normal) throw(): A(a), B(b), C(c), n(normal)
    {
    }

/*    Point intersect(const Ray r) throw(NoInterection)
    {
        float distance = 0;

        return Point(0,0,0);
    }*/
}