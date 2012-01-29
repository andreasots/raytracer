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
        SIMD::Matrix tangentSpace(RT_FLOAT u, RT_FLOAT v) const;
        RT_FLOAT intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v) const;
        SIMD::AABox bounds() const;
        void normals(const SIMD::Vec &n1,
                     const SIMD::Vec &n2,
                     const SIMD::Vec &n3);
    protected:
#ifdef __SSE4_1__
// Yet faster Ray-Triangle Intersection (Using SSE4)
// Jiri Havel and Adam Herout
        SIMD::Vec n, n1, n2;
#else
// Ray-Triangle Intersection Algorithm for Modern CPU Architectures
// Maxim Shevtsov, Alexei Soupikov and Alexander Kapustin
        RT_FLOAT nu, nv, np, pu, pv, e0u, e0v, e1u, e1v;
        int ci;
#endif
        SIMD::Vec m_normals[3];
        SIMD::AABox m_bbox;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_TRI_H
