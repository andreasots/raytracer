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
        Object(Material *mat): m_mat(mat)
        {
        }
        /** Default destructor */
        virtual ~Object()
        {

        }
        const Material *material()
        {
            return m_mat;
        }
        virtual SIMD::Vec normal(RT_FLOAT u, RT_FLOAT v) = 0;
        virtual RT_FLOAT intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v) = 0;
        virtual SIMD::AABox bounds() = 0;
    protected:
        Material *m_mat;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_OBJECT_H
