#include "raytracer/octree/root.h"
#include "raytracer/allocator.h"

namespace Raytracer
{
    namespace Octree
    {
        Root::Root(): m_bounds(SIMD::Point(), SIMD::Point()), tree(NULL)
        {
        }

        Root::~Root()
        {
        }

        RT_FLOAT Root::intersect(const SIMD::Ray &r, std::size_t &id, RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max,
                                 const std::vector<Raytracer::Object*> &objects,
                                 unsigned long long &intersections, unsigned long long &hits)
        {
            if(!tree)
                return max;

            SIMD::Point T1, T2;

            T1 = _mm_mul_ps((m_bounds.min()-r.origin).data(), r.inv_d.data());
            T2 = _mm_mul_ps((m_bounds.max()-r.origin).data(), r.inv_d.data());

            return tree->intersect(r, SIMD::AABox(T1, T2), id, u, v, max, objects, intersections, hits);
        }

        void Root::addCylinder(const SIMD::Point &a, const SIMD::Point &b, const RT_FLOAT r, std::size_t id)
        {
            SIMD::Vec R = b - a;
            R.normalize();
            R = _mm_mul_ps(R.data(), R.data());
            R = _mm_sub_ps(_mm_set1_ps(1), R.data());
            R = _mm_sqrt_ps(R.data());
            R = r*R;

            SIMD::AABox cyl(SIMD::Point(_mm_min_ps(a.data(), b.data())) - R, SIMD::Point(_mm_max_ps(a.data(), b.data())) + R);

            m_objects.push_back(std::make_tuple(cyl, id));
        }

        void Root::addQuad(const SIMD::Point &v00, const SIMD::Point &v01, const SIMD::Point &v11, const SIMD::Point &v10, std::size_t id)
        {
            SIMD::AABox bounds(v00, v00);
            bounds.extend(SIMD::AABox(v00, v00));
            bounds.extend(SIMD::AABox(v01, v01));
            bounds.extend(SIMD::AABox(v11, v11));
            bounds.extend(SIMD::AABox(v10, v10));

            m_objects.push_back(std::make_tuple(bounds, id));
        }

        void Root::addSphere(const SIMD::Point &c, RT_FLOAT r, std::size_t id)
        {
            SIMD::Vec R(r, r, r);

            m_objects.push_back(std::make_tuple(SIMD::AABox(c-R, c+R), id));
        }

        void Root::addTri(const SIMD::Point &a, const SIMD::Point &b, const SIMD::Point &c, std::size_t id)
        {
            SIMD::AABox bounds(a, a);
            bounds.extend(SIMD::AABox(b, b));
            bounds.extend(SIMD::AABox(c, c));

            m_objects.push_back(std::make_tuple(bounds, id));
        }

        void Root::rebuild()
        {
            deallocate<Node, 16>(tree, 1);
            tree = NULL;
            if(!m_objects.size())
                return;
            m_bounds = std::get<0>(m_objects.front());
            for(std::size_t i = 1; i < m_objects.size(); i++)
                m_bounds.extend(std::get<0>(m_objects[i]));
            tree = new (allocate<Node, 16>(1)) Node;
            for(std::size_t i = 0; i < m_objects.size(); i++)
                tree->add(m_bounds, m_objects[i]);
            tree->optimize();
        }
    } // namespace Octree
} // namespace Raytracer
