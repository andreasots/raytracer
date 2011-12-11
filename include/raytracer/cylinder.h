#ifndef RAYTRACER_CYLINDER_H
#define RAYTRACER_CYLINDER_H

#include "object.h"

namespace Raytracer {

class Cylinder : public Object
{
    public:
        Cylinder(SIMD::Point A, SIMD::Point B, RT_FLOAT r, Material mat);
        /** Default destructor */
        virtual ~Cylinder();
        const Material &material(RT_FLOAT u, RT_FLOAT v);
        SIMD::Vec normal(RT_FLOAT u, RT_FLOAT v);
        RT_FLOAT intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v);
        SIMD::AABox bounds();
    protected:
        SIMD::Point m_A;
        SIMD::Vec m_a;
        RT_FLOAT m_d;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_CYLINDER_H