#include "point.h"
#include "vector.h"
#include <ostream>

namespace Raytracer
{
    Point::Point(float x, float y, float z, float w): mX(x), mY(y), mZ(z), mW(w)
    {
        
    }

    Point::Point(float x, float y, float z): mX(x), mY(y), mZ(z), mW(1)
    {
    }

    Point::~Point()
    {
    }

    float Point::x() const
    {
        return this->mX;
    }

    float Point::y() const
    {
        return this->mY;
    }

    float Point::z() const
    {
        return this->mZ;
    }

    float Point::w() const
    {
        return this->mW;
    }

    Point Point::operator+(const Vector v) const
    {
        Point p = *this;
        p += v;
        return p;
    }

    Point &Point::operator+=(const Vector v)
    {
        this->mX += v.x();
        this->mY += v.y();
        this->mZ += v.z();
        return *this;
    }

    std::ostream& operator<<(std::ostream& out, Point p)
    {
        return out << "(" << p.x() << ", " << p.y() << ", " <<p.z()<<")";
    }

    Vector Point::operator-(const Point p) const
    {
        return Vector(p.x()-this->mX, p.y()-this->mY, p.z()-this->mZ); 
    }
}
