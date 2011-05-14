#include "point.h"
#include "vector.h"
#include "triangle.h"
#include <cmath>

namespace Raytracer
{
    Triangle::Triangle(Point a, Point b, Point c): A(a),B(b),C(c), n(0,0,0)
    {
        n = (b-a)*(c-a);
        n /= sqrt(n*n);
    }
    
    Triangle::Triangle(Point a, Point b, Point c, Vector normal):A(a),B(b),C(c),
                                                                 n(normal)
    {
    }
}