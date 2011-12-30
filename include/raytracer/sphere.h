#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "raytracer/object.h"
#include <SIMD/Point.h>

namespace Raytracer {

class Sphere : public Raytracer::Object
{
    public:
        /** Default constructor */
        Sphere(SIMD::Point o, RT_FLOAT r, Material *mat);
        /** Default destructor */
        virtual ~Sphere();
        virtual SIMD::Matrix tangentSpace(RT_FLOAT u, RT_FLOAT v);
        virtual RT_FLOAT intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v);
        virtual SIMD::AABox bounds();
    protected:
        SIMD::Point mO;
        RT_FLOAT mR;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_SPHERE_H
