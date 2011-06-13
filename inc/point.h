#ifndef RAYTRACER_POINT_H
#define RAYTRACER_POINT_H
#include <ostream>
namespace Raytracer
{
    class Vector;
    class Point
    {
        public:
            Point(float x, float y, float z);
            virtual ~Point();
            Point operator+(const Vector v) const;
            Point &operator+=(const Vector v);
            Vector operator-(const Point p) const;
            friend std::ostream& operator<<(std::ostream& out, Point p);
        protected:
            float mX, mY, mZ, mW;
    };
}
#endif
