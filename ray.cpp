#include "point.h"
#include "vector.h"
#include "ray.h"

namespace Raytracer
{
    Ray::Ray(Point start, Vector direction): mStart(start),mDirection(direction)
    {
    }

    Point Ray::start() throw()
    {
        return this->mStart;
    }

    Vector Ray::direction() throw()
    {
        return this->mDirection;
    }
}