#ifndef RAYTRACER_TRI_H
#define RAYTRACER_TRI_H

#include <gmtl/Tri.h>
#include "raytracer/object.h"

namespace Raytracer {

class Tri : public Object, public gmtl::Tri<FLOAT>
{
    public:
        /** Default constructor */
        Tri(const gmtl::Point<FLOAT, 3>& p1, const gmtl::Point<FLOAT, 3>& p2, const gmtl::Point<FLOAT, 3>& p3, Material mat);
        /** Default destructor */
        virtual ~Tri();
        virtual const Material &material(const gmtl::Point<FLOAT, 3> &p);
        virtual const Material &material()
        {
            return m_mat;
        }
        virtual gmtl::Vec<FLOAT, 3> normal(const gmtl::Point<FLOAT, 3> &p);
        virtual FLOAT intersect(const gmtl::Ray<FLOAT> &r);
        virtual gmtl::AABox<FLOAT> bounds();
        virtual gmtl::Point<FLOAT, 3> random();
        void normals(const gmtl::Vec<FLOAT, 3> &n1, const gmtl::Vec<FLOAT, 3> &n2, const gmtl::Vec<FLOAT, 3> &n3);
    protected:
        bool m_hasNormals;
        gmtl::Vec<FLOAT, 3> m_normals[3];

        FLOAT m_n_u, m_n_v, m_n_d;
        char m_k;
        FLOAT m_b_nu, m_b_nv, m_c_nu, m_c_nv;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_TRI_H
