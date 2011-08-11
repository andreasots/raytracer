#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "raytracer/object.h"

#include <gmtl/Sphere.h>

namespace Raytracer {

class Sphere : public Raytracer::Object, public gmtl::Sphere<FLOAT>
{
    public:
        /** Default constructor */
        Sphere(gmtl::Point<FLOAT, 3> o, FLOAT r, Material mat);
        /** Default destructor */
        virtual ~Sphere();
        virtual const Material &material(const gmtl::Point<FLOAT, 3> &p);
        virtual const Material &material()
        {
            return m_mat;
        }
        virtual gmtl::Vec<FLOAT, 3> normal(const gmtl::Point<FLOAT, 3> &p);
        virtual FLOAT intersect(const gmtl::Ray<FLOAT> &r);
        virtual gmtl::AABox<FLOAT> bounds();
        virtual gmtl::Point<FLOAT, 3> random();
    protected:
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_SPHERE_H
