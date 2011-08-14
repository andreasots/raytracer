#include "raytracer/scene.h"

#include "raytracer/sphere.h"
#include "raytracer/tri.h"

#include <gmtl/Containment.h>
#include <gmtl/Intersection.h>
#include <gmtl/TriOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Output.h>

#include <assimp/aiConfig.h>
#include <assimp/aiMesh.h>
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>
#include <assimp/assimp.hpp>

#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <tuple>
#include <stdexcept>

#define MAXDEPTH 128
#define SAMPLES 2
#define EPS static_cast<FLOAT>(0.001)

inline Raytracer::Color<> BRDF(const gmtl::Vec<FLOAT, 3> &in,
                               const gmtl::Vec<FLOAT, 3> &out,
                               const gmtl::Vec<FLOAT, 3> &n,
                               const Raytracer::Material &mat,
                               const Raytracer::Color<> &lambda)
{
    Raytracer::Color<> ret;

    // Diffuse reflection
    Raytracer::Color<> diffuse = lambda;
    diffuse.mult(gmtl::dot(in, n));
    diffuse.mult(M_1_PI);
    diffuse.mult(1-mat.specular);
    diffuse.mult(mat.color);
    ret.add(diffuse);

    // Specular reflection
    gmtl::Vec<FLOAT, 3> H = in+out;
    gmtl::normalize(H);
    Raytracer::Color<> specular = lambda;
    specular.mult(std::pow(std::max<FLOAT>(0,gmtl::dot(H, n)), mat.spec_pow));
    specular.mult((mat.spec_pow+8)/(M_PI*8));
    specular.mult(mat.specular);
    ret.add(specular);

    return ret;
}

inline gmtl::Vec<FLOAT, 3> hemisphere(const gmtl::Vec<FLOAT, 3> &n)
{
    gmtl::Vec<FLOAT, 3> ret;
    gmtl::Point<FLOAT, 3> origin(0,0,0);
    Raytracer::Sphere s(origin, 1, Raytracer::Material());

    ret = s.random()-origin;
    if(gmtl::dot(n, ret) < 0)
        ret = -ret;

    return ret;
}

namespace Raytracer {

Scene::Scene(): m_objects(), m_intersections(0), m_hits(0), m_octree(NULL)
{
}

void Scene::open(std::string file)
{
        Assimp::Importer importer;
    int pp = aiProcess_Triangulate;
    pp |= aiProcess_JoinIdenticalVertices | aiProcess_FindInvalidData;
    pp |= aiProcess_FindDegenerates | aiProcess_SortByPType;
    pp |= aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices;
    pp |= aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes;
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                                aiPrimitiveType_POINT | aiPrimitiveType_LINE);
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
                               aiComponent_BONEWEIGHTS|aiComponent_ANIMATIONS);
    importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80);
    const aiScene *scene = importer.ReadFile(file, pp);

    for(size_t i = 0; i < scene->mNumMeshes; i++)
    {
        Material mat;
        aiMaterial *m = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
        aiString name;
        m->Get(AI_MATKEY_NAME, name);
        aiColor3D c;
        m->Get(AI_MATKEY_COLOR_DIFFUSE, c);
        mat.color = Color<>(c.r, c.g, c.b);
        m->Get(AI_MATKEY_SHININESS, mat.spec_pow);

        std::cout << "Material '" << name.data << "': ";
        if(std::string(name.data).substr(0, 5) == "LIGHT")
        {
            mat.emit = true;
            std::cout << "emissive" << std::endl;
        }
        else if(std::string(name.data).substr(0, 7) == "REFLECT")
        {
            mat.reflect = 0.75;
            std::cout << "reflective" << std::endl;
        }
        else
            std::cout << "regular" << std::endl;

        for(size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
        {
            assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
            aiVector3D v(scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]]);
            gmtl::Point<FLOAT, 3> A(v.x, v.y, v.z);
            v = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]];
            gmtl::Point<FLOAT, 3> B(v.x, v.y, v.z);
            v = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]];
            gmtl::Point<FLOAT, 3> C(v.x, v.y, v.z);
            Tri *tri = new Tri(A, B, C, mat);
            v = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[0]];
            aiVector3D u(scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[1]]);
            aiVector3D w(scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[2]]);
            tri->normals(gmtl::Vec<FLOAT, 3>(v.x, v.y, v.z), gmtl::Vec<FLOAT, 3>(u.x, u.y, u.z), gmtl::Vec<FLOAT, 3>(w.x, w.y, w.z));
            add(tri);
        }
    }
    this->regenerate();
}

void Scene::add(Object *o)
{
    m_objects.push_back(o);
}

FLOAT Scene::intersect(const gmtl::Ray<FLOAT> &r, size_t &id, FLOAT max)
{
    FLOAT ret = max;

    if(m_octree)
        m_octree->intersect(r, ret, id, m_objects, m_hits, m_intersections);

    return ret;
}

Color<> Scene::radiance(const gmtl::Ray<FLOAT> &r, size_t depth)
{
    Color<> c;
    if(depth > MAXDEPTH || static_cast<FLOAT>(MAXDEPTH)/(depth*depth) < drand48())
        return Color<>();
    size_t id = 0;
    FLOAT t = this->intersect(r, id);
    if(t < HUGE_VAL)
    {
        gmtl::Point<FLOAT, 3> p = r.getOrigin()+t*r.getDir();
        Object *o = m_objects[id];
        Material mat = o->material(p);
        gmtl::Vec<FLOAT, 3> n = o->normal(p);

        if(mat.emit)
            return mat.color;

        for(auto i = m_emit.begin(); i != m_emit.end(); i++)
        {
            gmtl::Vec<FLOAT, 3> dir = m_objects[*i]->random() - p;
            FLOAT T = gmtl::normalize(dir);

            if(intersect(gmtl::Ray<FLOAT>(p, dir), id, T+EPS) == HUGE_VAL || *i != id)
                continue;
            c.add(BRDF(dir, -r.getDir(), n, mat, m_objects[*i]->material(p+t*dir).color));
        }

        gmtl::Vec<FLOAT, 3> dir = hemisphere(n);
        c.add(BRDF(dir, -r.getDir(), n, mat, radiance(gmtl::Ray<FLOAT>(p, dir), depth+1)));
    }

    return c;
}

void Scene::stats()
{
    std::cout << "\nSTATS" << std::endl;
    std::cout << "Number of primitives: " << m_objects.size() << std::endl;
    std::cout << "Number of intersection tests: " << m_intersections << std::endl;
    std::cout << "Number of successful intersection tests: " << m_hits << std::endl;
}

void Scene::regenerate()
{
    if(m_octree)
        delete m_octree;
    m_emit.clear();
    if(!(m_objects.size()))
        return;

    gmtl::AABox<FLOAT> box;

    for(size_t i = 0; i < m_objects.size(); i++)
    {
        gmtl::extendVolume(box, m_objects[i]->bounds());
        if(m_objects[i]->material().emit)
            m_emit.push_back(i);
    }

    std::cout << std::endl;
    std::cout << "Creating Octree: min: " << box.getMin() << std::endl;
    std::cout << "                 max: " << box.getMax() << std::endl;
    std::cout << std::endl;

    m_octree = new Octree(box.getMin(), box.getMax(), NULL);

    for(size_t i = 0; i < m_objects.size(); i++)
        m_octree->add(m_objects[i], i);

    m_octree->prune();
}
} // namespace Raytracer
