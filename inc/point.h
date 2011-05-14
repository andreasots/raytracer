#ifndef RAYTRACER_POINT_H
#define RAYTRACER_POINT_H
#include <ostream>
namespace Raytracer
{
    class Vector;
    class Point
    {
        public:
            Point(double x, double y, double z, double w);
            Point(double x, double y, double z);
            virtual ~Point();
            double x() const;
            double y() const;
            double z() const;
            double w() const;
            Point operator+(const Vector v) const;
            Point &operator+=(const Vector v);
            Vector operator-(const Point p) const;
            friend std::ostream& operator<<(std::ostream& out, Point p);
        protected:
            double mX, mY, mZ, mW;
    };
}
#endif
