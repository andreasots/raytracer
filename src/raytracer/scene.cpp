#include "raytracer/scene.h"

#include <gmtl/Intersection.h>
#include <gmtl/TriOps.h>
#include <gmtl/VecOps.h>

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
    ret.add(diffuse);

    // Specular reflection
    gmtl::Vec<FLOAT, 3> R;
    gmtl::reflect(R, in, n);
    Raytracer::Color<> specular = lambda;
    specular.mult(std::pow(gmtl::dot(R, out), mat.spec_pow));
    specular.mult((mat.spec_pow+2)/(M_PI*2));
    diffuse.mult(mat.specular);
    ret.add(specular);

    return ret;
}

inline FLOAT radius(const gmtl::Tri<FLOAT> &tri)
{
    FLOAT ret = HUGE_VAL;
    gmtl::Point<FLOAT, 3> c = gmtl::center(tri);
    for(size_t i = 0; i < 3; i++)
    {
        FLOAT l = gmtl::lengthSquared(gmtl::Vec<FLOAT, 3>(tri[i]-c));
        if(l > ret)
            ret = l;
    }
    return std::sqrt(ret);
}

inline gmtl::Point<FLOAT, 3> sphere(const FLOAT radius)
{
    FLOAT theta = 2*M_PI*drand48();
    FLOAT cos_theta = std::cos(theta), sin_theta = std::sin(theta);
    FLOAT cos_phi = drand48()*2-1;
    FLOAT sin_phi = std::sqrt(1-cos_phi*cos_phi);
    return gmtl::Point<FLOAT, 3>(radius*sin_phi*cos_theta, radius*sin_phi*sin_theta, radius*cos_phi);
}

inline gmtl::Vec<FLOAT, 3> hemisphere(const gmtl::Vec<FLOAT, 3> &n)
{
    gmtl::Vec<FLOAT, 3> ret;
    gmtl::Point<FLOAT, 3> origin(0,0,0);

    do
        ret = sphere(1)-origin;
    while(gmtl::dot(n, ret) < 0);

    return ret;
}

namespace Raytracer {

Scene::Scene(std::string file): m_planes(), m_spheres(), m_tris(), m_intersections(0), m_hits(0)
{
    Assimp::Importer importer;
    int pp = aiProcess_Triangulate;
    pp |= aiProcess_JoinIdenticalVertices | aiProcess_FindInvalidData;
    pp |= aiProcess_FindDegenerates | aiProcess_SortByPType;
    pp |= aiProcess_RemoveRedundantMaterials;
    pp |= aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices;
    pp |= aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes;
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                                aiPrimitiveType_POINT | aiPrimitiveType_LINE);
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
                               aiComponent_BONEWEIGHTS|aiComponent_ANIMATIONS);
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
        if(std::string(name.data).substr(0, 5) == "LIGHT")
            mat.emit = true;

        for(size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
        {
            assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
            aiVector3D v(scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]]);
            gmtl::Point<FLOAT, 3> A(v.x, v.y, v.z);
            v = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]];
            gmtl::Point<FLOAT, 3> B(v.x, v.y, v.z);
            v = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]];
            gmtl::Point<FLOAT, 3> C(v.x, v.y, v.z);
            add(gmtl::Tri<FLOAT>(A, B, C), mat);
        }
    }
    octreeRegen();
}

void Scene::add(gmtl::Sphere<FLOAT> sphere, Material mat)
{
    this->m_spheres.push_back(std::make_pair(sphere, mat));
}

void Scene::add(gmtl::Plane<FLOAT> plane, Material mat)
{
    this->m_planes.push_back(std::make_pair(plane, mat));
}

void Scene::add(gmtl::Tri<FLOAT> tri, Material mat)
{
    this->m_tris.push_back(std::make_pair(tri, mat));
}

FLOAT Scene::intersect(gmtl::Ray<FLOAT> r, ObjectType &type, size_t &id)
{
    FLOAT ret = HUGE_VAL;

    unsigned numhits;
    FLOAT t0 = 0, t1 = 0;
    if(!gmtl::intersect(m_bbox, r, numhits, t0, t1))
        return ret;

    for(size_t i = 0; i < m_spheres.size(); i++)
    {
        int hits;
        if(gmtl::intersect(m_spheres[i].first, r, hits, t0, t1))
        {
            assert(hits > 0);
            if(t0 < ret && t0 > 0)
            {
                ret = t0;
                type = SPHERE;
                id = i;
            }
            if(t1 < ret && t1 > 0)
            {
                ret = t1;
                type = SPHERE;
                id = i;
            }
            m_hits++;
        }
        m_intersections++;
    }

    for(size_t i = 0; i < m_planes.size(); i++)
    {
        FLOAT t;
        if(gmtl::intersect(m_planes[i].first, r, t) && t < ret && t > 0)
        {
            ret = t;
            type = PLANE;
            id = i;
            m_hits++;
        }
        m_intersections++;
    }

    for(size_t i = 0; i < m_tris.size(); i++)
    {
        float t, u, v;
        if(gmtl::intersect(m_tris[i].first, r, u, v, t) && t < ret && t > 0)
        {
            ret = t;
            type = TRI;
            id = i;
            m_hits++;
        }
        m_intersections++;
    }

    return ret;
}

Color<> Scene::radiance(gmtl::Ray<FLOAT> r, size_t depth)
{
    if(depth > MAXDEPTH)
        return Color<>();
    size_t id = 0;
    ObjectType type;
    FLOAT t = this->intersect(r, type, id);
    Color<> c;
    if(t < HUGE_VAL)
    {
        Material mat;
        gmtl::Vec<FLOAT, 3> n;
        gmtl::Point<FLOAT, 3> p = r.getOrigin()+t*r.getDir();

        switch(type)
        {
            case SPHERE:
                mat = m_spheres[id].second;
                n = p - m_spheres[id].first.getCenter();
                gmtl::normalize(n);
                break;
            case PLANE:
                mat = m_planes[id].second;
                n = m_planes[id].first.getNormal();
                break;
            case TRI:
                mat = m_tris[id].second;
                n = gmtl::normal(m_tris[id].first);
                break;
            default:
                throw std::out_of_range("Scene::radiance");
        }

// Texture mapping
/*        if(type == TRI)
        {
            float T, u, v;
            gmtl::intersect(m_tris[id].first, r, u, v, T);
            mat.color = Color<>(u, v, 1-u-v);
        }*/

//      return mat.color;

        if(mat.emit)
            c.add(mat.color);

        for(size_t i = 0; i < m_spheres.size(); i++)
        {
            if(!m_spheres[i].second.emit)
                continue;
            gmtl::Vec<FLOAT, 3> dir = m_spheres[i].first.getCenter()+sphere(m_spheres[i].first.getRadius()*drand48())-p;
            gmtl::normalize(dir);
            if(!(intersect(gmtl::Ray<FLOAT>(p+EPS*dir, dir), type, id) && getMat(type, id).emit))
                continue;
            c.add(radiance(gmtl::Ray<FLOAT>(p+EPS*dir, dir), depth+1).mult(M_1_PI));
        }

        for(size_t i = 0; i < m_tris.size(); i++)
        {
            if(!m_tris[i].second.emit)
                continue;
            gmtl::Vec<FLOAT, 3> dir = gmtl::center(m_tris[i].first)+sphere(radius(m_tris[i].first)*drand48())-p;
            gmtl::normalize(dir);
            if(!(intersect(gmtl::Ray<FLOAT>(p+EPS*dir, dir), type, id) && getMat(type, id).emit))
                continue;
            c.add(radiance(gmtl::Ray<FLOAT>(p+EPS*dir, dir), depth+1).mult(M_1_PI));
        }

        gmtl::Vec<FLOAT, 3> dir = hemisphere(n);
        Color<> lambda = radiance(gmtl::Ray<FLOAT>(p+EPS*dir, dir), depth+1);
        lambda.mult(gmtl::dot(dir, n));
        lambda.mult(M_1_PI);
        c.add(lambda);

        c.mult(mat.color);
    }

    return c;
}

void Scene::stats()
{
    std::cout << "\nSTATS" << std::endl;
    std::cout << "Number of primitives: " << m_spheres.size()+m_planes.size()+m_tris.size() << std::endl;
    std::cout << "Number of intersection tests: " << m_intersections << std::endl;
    std::cout << "Number of successful intersection tests: " << m_hits << std::endl;
}

Material Scene::getMat(ObjectType type, size_t id)
{
    switch(type)
    {
        case SPHERE:
            return m_spheres[id].second;
        case PLANE:
            return m_planes[id].second;
        case TRI:
            return m_tris[id].second;
        default:
            throw std::out_of_range("Scene::getMat");
    }
}

void Scene::octreeRegen()
{
    if(m_octree)
        delete m_octree;
    if(!(m_tris.size()+m_spheres.size()))
        return;

    gmtl::Point<FLOAT, 3> min(HUGE_VAL, HUGE_VAL, HUGE_VAL);
    gmtl::Point<FLOAT, 3> max(-HUGE_VAL, -HUGE_VAL, -HUGE_VAL);

    for(size_t i = 0; i < m_tris.size(); i++)
    {
        for(size_t j = 0; j < 3; j++)
        {
            for(size_t k = 0; k < 3; k++)
            {
                if(min[k] > m_tris[i].first[j][k])
                    min[k] = m_tris[i].first[j][k];
                if(max[k] < m_tris[i].first[j][k])
                    min[k] = m_tris[i].first[j][k];
            }
        }
    }

    for(size_t i = 0; i < m_spheres.size(); i++)
    {
        FLOAT r = m_spheres[i].first.getRadius();
        for(size_t j = 0; j < 3; j++)
        {
            if(min[j] > m_spheres[i].first.getCenter()[j]-r)
                min[j] = m_spheres[i].first.getCenter()[j]-r;
            if(max[j] < m_spheres[i].first.getCenter()[j]+r)
                max[j] = m_spheres[i].first.getCenter()[j]+r;
        }
    }

    m_octree = new Octree(min, max, NULL);

    for(size_t i = 0; i < m_tris.size(); i++)
        m_octree->add(m_tris[i].first, i);

    for(size_t i = 0; i < m_spheres.size(); i++)
        m_octree->add(m_spheres[i].first, i);

    m_octree->prune();
}
} // namespace Raytracer
