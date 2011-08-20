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
        virtual const Material &material(const gmtl::Point<FLOAT, 3> &p) = 0;
        virtual const Material &material() = 0;
        virtual gmtl::Vec<FLOAT, 3> normal(const gmtl::Point<FLOAT, 3> &p) = 0;
        virtual FLOAT intersect(const gmtl::Ray<FLOAT> &r) = 0;
        virtual gmtl::AABox<FLOAT> bounds() = 0;
        virtual gmtl::Point<FLOAT, 3> random() = 0;
    protected:
        Material m_mat;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_OBJECT_H
