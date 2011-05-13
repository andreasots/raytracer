#include "vector.h"
#define SQ(x) (x)*(x)

namespace Raytracer
{
  Vector Vector::operator-() const
  {
    Vector ret = *this;
    ret.x = -ret.x;
    ret.y = -ret.y;
    ret.z = -ret.z;
    return ret;
  }
  
  Vector Vector::operator+(const Vector v) const
  {
    Vector ret = *this;
    ret += v;
    return ret;
  }
  
  Vector Vector::operator-(const Vector v) const
  {
    Vector ret = *this;
    ret += -v;
    return ret;
  }
  
  Vector Vector::operator*(const double i) const
  {
    Vector ret = *this;
    ret *= i;
    return ret;
  }
  
  Vector Vector::operator/(const double i) const
  {
    Vector ret = *this;
    ret *= 1/i;
    return ret;
  }
  
  Vector Vector::&operator+=(const Vector v)
  {
    *this->mX += v.x();
    *this->mY += v.y();
    *this->mZ += v.z();
  }
  
  Vector Vector::&operator-=(const Vector v)
  {
    *this += -v;
  }
  
  Vector Vector::&operator*=(const double i)
  {
    *this->mX *= i;
    *this->mY *= i;
    *this->mZ *= i;
  }
  
  Vector Vector::&operator/=(const double i)
  {
    *this *= 1/i;
  }
  
  bool Vector::operator==(const Vector v) const
  {
    return (SQ(this->x())+SQ(this->y())+SQ(this->z())) == 
           (SQ(v.x())+SQ(v.y())+SQ(v.z()));
  }
  
  bool Vector::operator<(const Vector v) const
  {
      return (SQ(this->x())+SQ(this->y())+SQ(this->z())) <
             (SQ(v.x())+SQ(v.y())+SQ(v.z()));
  }
  
  double VecMult::double() const
  {
    double ret = this->mV.x()*this->mU.x();
    ret += this->mV.y()*this->mU.y();
    ret += this->mV.z()*this->mU.z();
    return ret;
  }
  
  Vector VecMult::Vector() const
  {
    double x, y, z;
    x = this->mV.y()*this->mU.z()-this->mV.z()*this->mU.y();
    y = this->mV.x()*this->mU.z()-this->mV.z()*this->mU.x();
    z = this->mV.x()*this->mU.y()-this->mV.y()*this->mU.x();
    return Vector(x, -y, z);
  }
  
  VecMult::VecMult()
  {
    throw Exception("This constructor must never be called");
  }
  
  VecMult::VecMult(Vector v, Vector u): mV(v), mU(u)
  {
  }
  
  friend VecMult operator*(Vector v, Vector u)
  {
    return VecMult(v, u);
  }
}
