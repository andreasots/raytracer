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

inline gmtl::Vec<FLOAT, 3> hemisphere(const gmtl::Vec<FLOAT, 3> &n)
{
    gmtl::Vec<FLOAT, 3> ret, x, y;

    do
        ret = gmtl::Vec<FLOAT, 3>((drand48()-0.5)*2, (drand48()-0.5)*2,(drand48()-0.5)*2);
    while(gmtl::dot(n, ret) < 0);

    gmtl::cross(x, n, ret);
    gmtl::cross(y, x, n);

    FLOAT r = drand48(), sin_a = 2*drand48()-1, cos_a = std::sqrt(1-sin_a*sin_a);

    ret = r*cos_a*x + r*sin_a*y + n;

    gmtl::normalize(ret);

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

    FLOAT max_x = -HUGE_VAL, max_y = -HUGE_VAL, max_z = -HUGE_VAL;
    FLOAT min_x = HUGE_VAL, min_y = HUGE_VAL, min_z = HUGE_VAL;

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
        {
            mat.emit = true;
            mat.color.mult(10.0/std::min(c.r, std::min(c.g, c.b)));
        }

        for(size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
        {
            assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
            aiVector3D v(scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]]);
            if(v.x > max_x)
                max_x = v.x;
            if(v.x < min_x)
                min_x = v.x;
            if(v.y > max_y)
                max_y = v.y;
            if(v.y < min_y)
                min_y = v.y;
            if(v.z > max_z)
                max_z = v.z;
            if(v.z < min_z)
                min_z = v.z;
            gmtl::Point<FLOAT, 3> A(v.x, v.y, v.z);
            v = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]];
            if(v.x > max_x)
                max_x = v.x;
            if(v.x < min_x)
                min_x = v.x;
            if(v.y > max_y)
                max_y = v.y;
            if(v.y < min_y)
                min_y = v.y;
            if(v.z > max_z)
                max_z = v.z;
            if(v.z < min_z)
                min_z = v.z;
            gmtl::Point<FLOAT, 3> B(v.x, v.y, v.z);
            v = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]];
            if(v.x > max_x)
                max_x = v.x;
            if(v.x < min_x)
                min_x = v.x;
            if(v.y > max_y)
                max_y = v.y;
            if(v.y < min_y)
                min_y = v.y;
            if(v.z > max_z)
                max_z = v.z;
            if(v.z < min_z)
                min_z = v.z;
            gmtl::Point<FLOAT, 3> C(v.x, v.y, v.z);
            add(gmtl::Tri<FLOAT>(A, B, C), mat);
        }
    }
    if(max_x < HUGE_VAL)
        m_bbox = gmtl::AABox<FLOAT>(gmtl::Point<FLOAT, 3>(min_x, min_y, min_z), gmtl::Point<FLOAT, 3>(max_x, max_y, max_z));
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

        if(mat.emit)
            c.add(mat.color);

        for(size_t i = 0; i < m_spheres.size(); i++)
        {
            if(!m_spheres[i].second.emit)
                continue;
            gmtl::Vec<FLOAT, 3> sw = m_spheres[i].first.getCenter() - p;
            gmtl::Vec<FLOAT, 3> su;
            if(std::abs(sw[0]) > .1)
                gmtl::cross(su, gmtl::Vec<FLOAT, 3>(0,1,0), sw);
            else
                gmtl::cross(su, gmtl::Vec<FLOAT, 3>(1,0,0), sw);
            gmtl::normalize(su);
            gmtl::Vec<FLOAT, 3> sv = gmtl::makeCross(sw, su);
            FLOAT cos_a_max = std::sqrt(1-std::pow(m_spheres[i].first.getRadius(), 2)/gmtl::lengthSquared(sw));
            FLOAT eps1 = drand48(), eps2 = drand48();
            FLOAT cos_a = 1-eps1+eps1*cos_a_max;
            FLOAT sin_a = std::sqrt(1-cos_a*cos_a);
            FLOAT phi = 2*M_PI*eps2;
            gmtl::Vec<FLOAT, 3> l = su*std::cos(phi)*sin_a+sv*std::sin(phi)*sin_a+sw*cos_a;
            gmtl::normalize(l);
            if(intersect(gmtl::Ray<FLOAT>(p, l), type, id) > HUGE_VAL && type == SPHERE && id == i)
            {
                Color<> e = mat.color;
                e.mult(m_spheres[i].second.color);
                e.mult(gmtl::dot(l, n));
                e.mult(2*(1-cos_a_max));
                c.add(e);
            }
        }
        for(size_t i = 0; i < m_tris.size(); i++);

        gmtl::Vec<FLOAT, 3> dir = hemisphere(n);
        Color<> lambda = radiance(gmtl::Ray<FLOAT>(p+EPS*dir, dir), depth+1);
        lambda.mult(gmtl::dot(dir, n));
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
} // namespace Raytracer
