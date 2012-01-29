#ifndef RAYTRACER_QUAD_H
#define RAYTRACER_QUAD_H

#include <SIMD/AABox.h>
#include "raytracer/object.h"

namespace Raytracer {

class Quad : public Object
{
    public:
        /** Default constructor */
        Quad(const SIMD::Point& A,
             const SIMD::Point& B,
             const SIMD::Point& C,
             const SIMD::Point& D,
             Material *mat);
        /** Default destructor */
        virtual ~Quad();
        SIMD::Matrix tangentSpace(RT_FLOAT u, RT_FLOAT v) const;
        RT_FLOAT intersect(const SIMD::Ray &r, RT_FLOAT &u, RT_FLOAT &v) const;
        SIMD::AABox bounds() const;
        void normals(const SIMD::Vec &n00,
                     const SIMD::Vec &n10,
                     const SIMD::Vec &n11,
                     const SIMD::Vec &n01);
    protected:
        SIMD::Point v00, v10, v11, v01;
        SIMD::Vec m_normals[4];
        RT_FLOAT a11, b11;
        RT_UINT flags;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_TRI_H
