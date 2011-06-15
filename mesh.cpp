#include <aiMesh.h>
#include "point.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include <cassert>

namespace Raytracer
{
    Mesh::Mesh(aiMesh mesh)
    {
/*        for(int i=0; i < mesh.mNumFaces; i++)
        {
            assert(mesh.mFaces[i].mNumIndexes == 3);
            Triangle tri(mes.mFaces[i].mIndexes
        }*/
    }

    TriangleIter Mesh::begin()
    {
        return mMesh.begin();
    }

    TriangleIter Mesh::end()
    {
        return mMesh.end();
    }
}
