#include "raytracer/scene.h"

#include "raytracer/sphere.h"
#include "raytracer/tri.h"

#include <SIMD/Vec.h>
#include <SIMD/Point.h>
#include <SIMD/AABox.h>

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

namespace Raytracer {

Scene::Scene(): m_objects(), m_intersections(0), m_hits(0), m_octree(NULL)
{
}

Scene::~Scene()
{
    delete m_octree;
    while(!m_objects.empty())
    {
        delete m_objects.back();
        m_objects.pop_back();
    }
}

void Scene::open(std::string file)
{
    Assimp::Importer importer;
    int pp = aiProcess_Triangulate | aiProcess_FindInvalidData;
    pp |= aiProcess_FindDegenerates | aiProcess_SortByPType;
    pp |= aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices;
    pp |= aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes;
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                                aiPrimitiveType_POINT | aiPrimitiveType_LINE);
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
                               aiComponent_BONEWEIGHTS|aiComponent_ANIMATIONS);
    importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 30);
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
        //Y = 0.2126 R + 0.7152 G + 0.0722 B
        m->Get(AI_MATKEY_COLOR_SPECULAR, c);
        mat.specular = 0.2126*c.r + 0.7152*c.g + 0.0722*c.b;

        std::cout << "Material '" << name.data << "': ";
        if(std::string(name.data).substr(0, 5) == "LIGHT")
        {
            mat.emit = true;
            std::cout << "emissive" << std::endl;
        }
        else
            std::cout << "regular" << std::endl;

        for(size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
        {
            assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
            aiVector3D v(scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]]);
            SIMD::Point A(v.x, v.y, v.z);
            v = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]];
            SIMD::Point B(v.x, v.y, v.z);
            v = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]];
            SIMD::Point C(v.x, v.y, v.z);
            Tri *tri = new Tri(A, B, C, mat);
            v = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[0]];
            aiVector3D u(scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[1]]);
            aiVector3D w(scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[2]]);
            tri->normals(SIMD::Vec(v.x, v.y, v.z), SIMD::Vec(u.x, u.y, u.z), SIMD::Vec(w.x, w.y, w.z));
            add(tri);
        }
    }
//    this->regenerate();
}

void Scene::add(Object *o)
{
    m_objects.push_back(o);
}

RT_FLOAT Scene::intersect(const SIMD::Ray &r, size_t &id, RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max)
{
    RT_FLOAT ret = max;

/*    if(m_octree)
        m_octree->intersect(r, ret, id, u, v, m_objects, m_hits, m_intersections);*/
    for(size_t i = 0; i < m_objects.size(); i++)
    {
        RT_FLOAT U, V;
        RT_FLOAT T = m_objects[i]->intersect(r, u, v);
        if(T < ret)
        {
            ret = T;
            id = i;
            u = U;
            v = V;
            m_hits++;
        }
        m_intersections++;
    }

    return ret;
}

Color<> Scene::radiance(const SIMD::Ray &r, size_t depth)
{
    Color<> c;
    if(depth > MAXDEPTH)
        return Color<>();
    size_t id;
    RT_FLOAT u, v;
    RT_FLOAT t = this->intersect(r, id, u, v);
    if(t < HUGE_VAL)
    {
        SIMD::Point p = r.origin+t*r.direction;
        std::clog << p[0] << "; "<< p[1] << "; "<< p[2] << ";" << std::endl;
        Object *o = m_objects[id];
        Material mat = o->material(u, v);
        SIMD::Vec n = o->normal(u, v);

        return Color<>((n[0]+1)/2, (n[1]+1)/2, (n[2]+1)/2);

        if(mat.emit)
            c.add(mat.color);

        if(n.dot(r.direction) < 0)
        {
            RT_FLOAT u = drand48();
            if(mat.diffuse > u)
                c.add(mat.diffuseCalc(radiance(SIMD::Ray(p, mat.diffuseSample(n)), depth+1)));
            else if(mat.diffuse+mat.specular > u)
                c.add(mat.specularCalc(radiance(SIMD::Ray(p, mat.specularSample(n, r.direction)), depth+1)));
        }
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
    if(!(m_objects.size()))
        return;

    SIMD::AABox box = m_objects[0]->bounds();

    for(size_t i = 1; i < m_objects.size(); i++)
        box.extend(m_objects[i]->bounds());

    std::cout << std::endl;
    std::cout << "Creating Octree: min: " << box.min() << std::endl;
    std::cout << "                 max: " << box.max() << std::endl;
    std::cout << std::endl;

    m_octree = new Octree(box.min(), box.max(), NULL);

    for(size_t i = 0; i < m_objects.size(); i++)
        m_octree->add(m_objects[i], i);

    m_octree->prune(m_objects);
}
} // namespace Raytracer
