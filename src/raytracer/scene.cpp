#include "raytracer/scene.h"

#include <gmtl/Intersection.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <tuple>
#include <stdexcept>

namespace Raytracer {

Scene::Scene(std::string file): m_intersections(0), m_hits(0)
{
    //ctor
    Material mat;

    mat.diffuse = Color<>(0.4f, 0.3f, 0.3f);
    this->add(gmtl::Plane<FLOAT>(gmtl::Vec<FLOAT,3>(0,1,0), 4.4f), mat);

    mat.diffuse = Color<>(0.7f, 0.7f, 0.7f);
    this->add(gmtl::Sphere<FLOAT>(gmtl::Point<FLOAT, 3>(1, -0.8f, 3), 2.5f), mat);
    this->add(gmtl::Sphere<FLOAT>(gmtl::Point<FLOAT, 3>(-5.5f, -0.5, 7), 2), mat);
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
    for(size_t i = 0; i < m_spheres.size(); i++)
    {
        int hits;
        FLOAT t0, t1;
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
    size_t id = 0;
    ObjectType type;
    FLOAT t = this->intersect(r, type, id);
    if(t < HUGE_VAL)
    {
        Material mat;
        switch(type)
        {
            case SPHERE:
                mat = m_spheres[id].second;
                break;
            case PLANE:
                mat = m_planes[id].second;
                break;
            case TRI:
                mat = m_tris[id].second;
                break;
            default:
                throw std::out_of_range("Scene::radiance");
        }
        return mat.diffuse;
    }
    else
        return Color<>();
}

void Scene::stats()
{
    std::cout << "\nSTATS" << std::endl;
    std::cout << "Number of primitives: " << m_spheres.size()+m_planes.size()+m_tris.size() << std::endl;
    std::cout << "Number of intersection tests: " << m_intersections << std::endl;
    std::cout << "Number of successful intersection tests: " << m_hits << std::endl;
}
} // namespace Raytracer
