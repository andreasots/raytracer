#ifndef RAYTRACER_TRI_H
#define RAYTRACER_TRI_H

#include <SIMD/AABox.h>
#include "raytracer/object.h"

namespace Raytracer {

class Tri : public Object
{
    public:
        /** Default constructor */
        Tri(const SIMD::Point& p1,
            const SIMD::Point& p2,
            const SIMD::Point& p3,
            Material *mat);
        /** Default destructor */
        virtual ~Tri();
        virtual SIMD::Vec normal(RT_FLOAT u, RT_FLOAT v);
        virtual RT_FLOAT intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v);
        virtual SIMD::AABox bounds();
        void normals(const SIMD::Vec &n1,
                     const SIMD::Vec &n2,
                     const SIMD::Vec &n3);
        RT_FLOAT nu, nv, np, pu, pv, e0u, e0v, e1u, e1v;
        int ci;
    protected:
        SIMD::Vec m_normals[3];
        SIMD::AABox *m_bbox;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_TRI_H
