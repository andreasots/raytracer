#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "raytracer/material.h"

#include <SIMD/Point.h>
#include <SIMD/Vec.h>
#include <SIMD/Ray.h>
#include <SIMD/AABox.h>

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
        virtual SIMD::Vec normal(RT_FLOAT u, RT_FLOAT v) = 0;
        virtual RT_FLOAT intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v) = 0;
        virtual SIMD::AABox bounds() = 0;
    protected:
        Material m_mat;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_OBJECT_H
