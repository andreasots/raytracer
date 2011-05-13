#ifndef RAYTRACER_POINT_H
#define RAYTRACER_POINT_H
#import "vector.h"
namespace Raytracer
{
  class Point
  {
    public:
      Point(double x, double y, double z, double w);
      Point(double x, double y, double z);
      double x() const;
      double y() const;
      double z() const;
      double w() const;
      Point operator+(const Vector v) const;
      Point &operator+=(const Vector v);
    private:
      double mX, mY, mZ, mW;
  };
}
#endif
