#include "vector.h"
#include "exception.h"
#include <cmath>
#define SQ(x) (x)*(x)

namespace Raytracer
{
  Vector::Vector(float x, float y, float z, float w): Point(x, y, z, w)
  {
  }
  
  Vector::Vector(float x, float y, float z): Point(x, y, z)
  {
  }
  
  Vector Vector::operator-() const
  {
    Vector ret = *this;
    ret.mX = -ret.mX;
    ret.mY = -ret.mY;
    ret.mZ = -ret.mZ;
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
  
  Vector Vector::operator*(const float i) const
  {
    Vector ret = *this;
    ret *= i;
    return ret;
  }
  
  Vector Vector::operator/(const float i) const
  {
    Vector ret = *this;
    ret *= 1/i;
    return ret;
  }
  
  Vector &Vector::operator+=(const Vector v)
  {
    this->mX += v.x();
    this->mY += v.y();
    this->mZ += v.z();
    return *this;
  }
  
  Vector &Vector::operator-=(const Vector v)
  {
    *this += -v;
    return *this;
  }
  
  Vector &Vector::operator*=(const float i)
  {
    this->mX *= i;
    this->mY *= i;
    this->mZ *= i;
    return *this;
  }
  
  Vector &Vector::operator/=(const float i)
  {
    *this *= 1/i;
    return *this;
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
    
  VecMult::operator float() const
  {
    float ret = this->mV.x()*this->mU.x();
    ret += this->mV.y()*this->mU.y();
    ret += this->mV.z()*this->mU.z();
    return ret;
  }
  
  VecMult::operator Vector() const
  {
    float x, y, z;
    x = this->mV.y()*this->mU.z()-this->mV.z()*this->mU.y();
    y = this->mV.x()*this->mU.z()-this->mV.z()*this->mU.x();
    z = this->mV.x()*this->mU.y()-this->mV.y()*this->mU.x();
    return Vector(x, -y, z);
  }
  
  VecMult::VecMult(): mV(0, 0, 0), mU(0, 0, 0)
  {
    throw Exception("This constructor must never be called");
  }
  
  VecMult::VecMult(Vector v, Vector u): mV(v), mU(u)
  {
  } 
}

Raytracer::VecMult operator*(const Raytracer::Vector v, const Raytracer::Vector u)
  {
    return Raytracer::VecMult(v, u);
  }

