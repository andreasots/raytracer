#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "raytracer/object.h"

namespace Raytracer {

class Sphere : public Object
{
    public:
        /** Default constructor */
        Sphere(Vector<> pos, Material mat, float r);
        virtual float intersect(Ray<> &r);
        virtual Vector<> normal(Vector<> pos);
    protected:
        float m_r2; //!< Member variable "m_r2"
};

} // namespace Raytracer

#endif // RAYTRACER_SPHERE_H
