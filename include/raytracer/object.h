#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "raytracer/material.h"

#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/Ray.h>
#include <gmtl/AABox.h>

namespace Raytracer {

class Object
{
    public:
        Object(Material mat): m_mat(mat)
        {
        }
        /** Default destructor */
        virtual ~Object()
        {

        }
        virtual const Material &material(RT_FLOAT u, RT_FLOAT v) = 0;
        virtual const Material &material() = 0;
        virtual gmtl::Vec<RT_FLOAT, 3> normal(RT_FLOAT u, RT_FLOAT v) = 0;
        virtual RT_FLOAT intersect(const gmtl::Ray<RT_FLOAT> &r, RT_FLOAT &u, RT_FLOAT &v) = 0;
        virtual gmtl::AABox<RT_FLOAT> bounds() = 0;
    protected:
        Material m_mat;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_OBJECT_H
