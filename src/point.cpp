#include "point.h"
namespace Raytracer
{
  Point::Point(double x, double y, double z, double w): mX(x), mY(y),mZ(z),mW(w)
  {
  }
   
  Point::Point(double x, double y, double z): mX(x), mY(y), mZ(z), mW(1)
  {
  }
   
   
  double Point::x() const
  {
    return this->mX;
  }
      
  double Point::y() const
  {
    return this->mY;
  }
      
  double Point::z() const
  {
    return this->mZ;
  }
      
  double Point::w() const
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
}
