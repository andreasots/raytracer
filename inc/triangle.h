#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H
#include "exception.h"
namespace Raytracer
{
    class Point;
    class Vector;
    class Ray;

    class NoIntersection: public Exception
    {
    };
    
    struct Triangle
    {
        Triangle(Point a, Point b, Point c) throw();
        Triangle(Point a, Point b, Point c, Vector normal) throw();

        Point intersect(const Ray r) throw(NoItersection);

        Point A, B, C;
        Vector n;
    };
}
#endif