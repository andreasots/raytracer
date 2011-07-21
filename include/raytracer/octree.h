#ifndef RAYTRACER_OCTREE_H
#define RAYTRACER_OCTREE_H

namespace Raytracer {

enum ObjectType
{
    PLANE,
    SPHERE,
    TRI
};

class Octree: public gmtl::AABox<FLOAT>
{
    public:
        Octree(const gmtl::Point<DATA_TYPE, 3>& min,
               const gmtl::Point<DATA_TYPE, 3>& max);
        bool add(gmtl::Tri<FLOAT> tri, size_t id);
        bool add(gmtl::Sphere<FLOAT> sphere, size_t id);
        std::vector<std::pair<ObjectType, size_t>> objects;
    protected:
        Octree *m_subnodes[8];
};

} // namespace Raytracer

#endif // RAYTRACER_OCTREE_H
