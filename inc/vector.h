#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H
#include "point.h"
#include <utility>
namespace Raytracer
{
  using namespace rel_ops; // For != > =< =>
  class Vector: public Point
  {
    public:
      Vector operator-() const;
      Vector operator+(const Vector v) const;
      Vector operator-(const Vector v) const;
      Vector operator*(const double i) const;
      Vector operator/(const double i) const;
      Vector &operator+=(const Vector v);
      Vector &operator-=(const Vector v);
      Vector &operator*=(const double i);
      Vector &operator/=(const double i);
      bool operator==(const Vector v) const;
      bool operator<(const Vector v) const;
  };
  
  class VecMult
  {
    public:
      double double() const;
      Vector Vector() const;
    private:
      VecMult();
      VecMult(Vector v, Vector u);
      Vector mV, mU;
  }
  
  friend VecMult operator*(Vector v, Vector u);
}
#endif
