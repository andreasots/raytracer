#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H
#include "point.h"
#include <utility>
using namespace std::rel_ops; // For != > =< =>
namespace Raytracer
{
  class Vector: public Point
  {
    public:
      Vector(float x, float y, float z, float w);
      Vector(float x, float y, float z);
      Vector operator-() const;
      Vector operator+(const Vector v) const;
      Vector operator-(const Vector v) const;
      Vector operator*(const float i) const;
      Vector operator/(const float i) const;
      Vector &operator+=(const Vector v);
      Vector &operator-=(const Vector v);
      Vector &operator*=(const float i);
      Vector &operator/=(const float i);
      bool operator==(const Vector v) const;
      bool operator<(const Vector v) const;
  };
  
  class VecMult
  {
    public:
      operator float() const;
      operator Vector() const;
      VecMult(Vector v, Vector u);
    private:
      VecMult();
      Vector mV, mU;
  };
}
Raytracer::VecMult operator*(const Raytracer::Vector v, const Raytracer::Vector u);
#endif
