#ifndef RAYTRACER_OCTREE_ROOT_H
#define RAYTRACER_OCTREE_ROOT_H

namespace Raytracer {
namespace Octree {

class Root
{
    public:
        Root(): m_bounds(SIMD::Point(), SIMD::Point())
        {
        }

        virtual ~Root()
        {
        }

        RT_FLOAT intersect(const SIMD::Ray &r, std::size_t &id, RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max,
                           const std::vector<Raytracer::Object*> &objects,
                           unsigned long long &intersections, unsigned long long &hits)
        {
            RT_FLOAT t0, t1;
            if(!m_bounds.intersect(r, t0, t1))
                return HUGE_VAL;

            RT_FLOAT ret = max;

            for(size_t i = 0; i < objects.size(); i++)
            {
                RT_FLOAT U, V;
                RT_FLOAT T = objects[i]->intersect(r, U, V);
                if(T < ret)
                {
                    ret = T;
                    id = i;
                    u = U;
                    v = V;
                    hits++;
                }
                intersections++;
            }
            return ret;
        }

        void addCylinder(const SIMD::Point &a, const SIMD::Point &b, const RT_FLOAT r, std::size_t id)
        {
            SIMD::Vec n = b-a;
            n.normalize();
            SIMD::Vec R(r*std::sqrt(1-n[0]*n[0]), r*std::sqrt(1-n[1]*n[1]), r*std::sqrt(1-n[2]*n[2]));

            SIMD::AABox cyl(SIMD::Point(_mm_min_ps(a.data(), b.data())) - R, SIMD::Point(_mm_max_ps(a.data(), b.data())) + R);
            if(m_bounds.empty())
                m_bounds = cyl;
            else
                m_bounds.extend(cyl);
        }

        void addQuad(const SIMD::Point &v00, const SIMD::Point &v01, const SIMD::Point &v11, const SIMD::Point &v10, std::size_t id)
        {
            if(m_bounds.empty())
                m_bounds = SIMD::AABox(v00, v00);
            else
                m_bounds.extend(SIMD::AABox(v00, v00));
            m_bounds.extend(SIMD::AABox(v01, v01));
            m_bounds.extend(SIMD::AABox(v11, v11));
            m_bounds.extend(SIMD::AABox(v10, v10));
        }

        void addSphere(const SIMD::Point &c, RT_FLOAT r, std::size_t id)
        {
            SIMD::Vec R(r, r, r);
            if(m_bounds.empty())
                m_bounds = SIMD::AABox(c-R, c+R);
            else
                m_bounds.extend(SIMD::AABox(c-R, c+R));
        }

        void addTri(const SIMD::Point &a, const SIMD::Point &b, const SIMD::Point &c, std::size_t id)
        {
            if(m_bounds.empty())
                m_bounds = SIMD::AABox(a, a);
            else
                m_bounds.extend(SIMD::AABox(a, a));
            m_bounds.extend(SIMD::AABox(b, b));
            m_bounds.extend(SIMD::AABox(c, c));
        }

    protected:
        SIMD::AABox m_bounds;
//        Node *tree;
    private:
};

} // namespace Octree
} // namespace Raytracer

#endif // RAYTRACER_OCTREE_ROOT_H
