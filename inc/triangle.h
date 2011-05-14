#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H
namespace Raytracer
{
    class Point;
    class Vector;
    struct Triangle
    {
        Triangle(Point a, Point b, Point c);
        Triangle(Point a, Point b, Point c, Vector normal);

        Point A, B, C;
        Vector n;
    };
}
#endif