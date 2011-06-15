#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H
#include <vector>
#include "triangle.h"
struct aiMesh;
namespace Raytracer
{
    typedef std::vector<Triangle>::iterator TriangleIter;
    class Mesh
    {
        public:
            Mesh(aiMesh mesh);
            TriangleIter begin();
            TriangleIter end();
        private:
            std::vector<Triangle> mMesh;
    };
}
#endif