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
      Vector(double x, double y, double z, double w);
      Vector(double x, double y, double z);
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
      operator double() const;
      operator Vector() const;
      VecMult(Vector v, Vector u);
    private:
      VecMult();
      Vector mV, mU;
  };
}
Raytracer::VecMult operator*(const Raytracer::Vector v, const Raytracer::Vector u);
#endif
