#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "raytracer/material.h"
#include "raytracer/ray.h"
#include "raytracer/vector.h"

namespace Raytracer {

class Object
{
    public:
        /** Default constructor */
        Object(Vector<> pos, Material mat);
        virtual float intersect(Ray<> &r);
        virtual Vector<> normal(Vector<> pos);
        Material material()
        {
            return m_mat;
        }
    protected:
        Vector<> m_pos;
        Material m_mat;
};

} // namespace Raytracer

#endif // RAYTRACER_OBJECT_H
