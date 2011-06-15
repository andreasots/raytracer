#include "point.h"
#include "vector.h"
#include <ostream>

namespace Raytracer
{
    Point::Point(float x, float y, float z): mX(x), mY(y), mZ(z), mW(1)
    {
    }

    Point::~Point()
    {
    }

    Point Point::operator+(const Vector v) const
    {
        Point p = *this;
        p += v;
        return p;
    }

    Point &Point::operator+=(const Vector v)
    {
        this->mX += v.mX;
        this->mY += v.mX;
        this->mZ += v.mX;
        return *this;
    }

    std::ostream& operator<<(std::ostream& out, Point p)
    {
        return out << "(" << p.mX << ", " << p.mY << ", " << p.mY <<")";
    }

    Vector Point::operator-(const Point p) const
    {
        return Vector(p.mX-this->mX, p.mY-this->mY, p.mZ-this->mZ); 
    }
}
