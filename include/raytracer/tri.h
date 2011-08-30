#ifndef RAYTRACER_TRI_H
#define RAYTRACER_TRI_H

#include <gmtl/AABox.h>
#include "raytracer/object.h"

namespace Raytracer {

class Tri : public Object
{
    public:
        /** Default constructor */
        Tri(const gmtl::Point<RT_FLOAT, 3>& p1, const gmtl::Point<RT_FLOAT, 3>& p2, const gmtl::Point<RT_FLOAT, 3>& p3, Material mat);
        /** Default destructor */
        virtual ~Tri();
        virtual const Material &material(RT_FLOAT u, RT_FLOAT v);
        virtual const Material &material()
        {
            return m_mat;
        }
        virtual gmtl::Vec<RT_FLOAT, 3> normal(RT_FLOAT u, RT_FLOAT v);
        virtual RT_FLOAT intersect(const gmtl::Ray<RT_FLOAT> &r, RT_FLOAT &u, RT_FLOAT &v);
        virtual gmtl::AABox<RT_FLOAT> bounds();
        void normals(const gmtl::Vec<RT_FLOAT, 3> &n1, const gmtl::Vec<RT_FLOAT, 3> &n2, const gmtl::Vec<RT_FLOAT, 3> &n3);
        RT_FLOAT nu, nv, np, pu, pv, e0u, e0v, e1u, e1v;
        int ci;
    protected:
        gmtl::Vec<RT_FLOAT, 3> m_normals[3];
        bool m_hasNormals;
        gmtl::AABox<RT_FLOAT> *m_bbox;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_TRI_H
