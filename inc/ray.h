#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H
namespace Raytracer
{
    class Point;
    class Vector;
    class Ray
    {
        public:
            Ray(Point start, Vector direction);
            Point start() throw();
            Vector direction() throw();
        private:
            Point mStart();
            Vector mDirection();
    }
}
#endif