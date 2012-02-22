#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "raytracer/material.h"

#include <SIMD/Point.h>
#include <SIMD/Vec.h>
#include <SIMD/Ray.h>
#include <SIMD/AABox.h>
#include <SIMD/Matrix.h>

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
        const Material *material() const
        {
            return m_mat;
        }
        virtual SIMD::Matrix tangentSpace(RT_FLOAT u, RT_FLOAT v) const = 0;
        virtual RT_FLOAT intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v) const = 0;
    protected:
        Material *m_mat;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_OBJECT_H
