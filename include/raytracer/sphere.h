#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "raytracer/object.h"

#include <gmtl/Sphere.h>

namespace Raytracer {

class Sphere : public Raytracer::Object, public gmtl::Sphere<RT_FLOAT>
{
    public:
        /** Default constructor */
        Sphere(gmtl::Point<RT_FLOAT, 3> o, RT_FLOAT r, Material mat);
        /** Default destructor */
        virtual ~Sphere();
        virtual const Material &material(RT_FLOAT u, RT_FLOAT v);
        virtual const Material &material()
        {
            return m_mat;
        }
        virtual gmtl::Vec<RT_FLOAT, 3> normal(RT_FLOAT u, RT_FLOAT v);
        virtual RT_FLOAT intersect(const gmtl::Ray<RT_FLOAT> &r, RT_FLOAT &u, RT_FLOAT &v);
        virtual gmtl::AABox<RT_FLOAT> bounds();
    protected:
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_SPHERE_H
