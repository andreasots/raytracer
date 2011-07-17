#include "raytracer/scene.h"

#include <gmtl/Intersection.h>
#include <gmtl/TriOps.h>
#include <gmtl/VecOps.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <tuple>
#include <stdexcept>

#define MAXDEPTH 128

inline Raytracer::Color<> diffuse(const gmtl::Vec<FLOAT, 3> &L,
                           const gmtl::Vec<FLOAT, 3> &N,
                           const Raytracer::Material &light,
                           const Raytracer::Material &prim)
{
    FLOAT dot = std::max<FLOAT>(gmtl::dot(L, N), 0);

    Raytracer::Color<> ret = prim.color;
    ret.mult(light.color);
    ret.mult(dot*(1-prim.reflect));

    return ret;
}

inline Raytracer::Color<> specular(const gmtl::Vec<FLOAT, 3> &L,
                           const gmtl::Vec<FLOAT, 3> &N,
                           const gmtl::Vec<FLOAT, 3> &V,
                           const Raytracer::Material &light,
                           const Raytracer::Material &prim)
{
    gmtl::Vec<FLOAT, 3> R;
    gmtl::reflect(R, L, N);
    FLOAT dot = std::pow(std::max<FLOAT>(gmtl::dot(V, R), 0), prim.spec_pow);

    Raytracer::Color<> ret = prim.color;
    ret.mult(light.color);
    ret.mult(dot*prim.specular);

    return ret;
}


namespace Raytracer {

Scene::Scene(std::string file): m_planes(), m_spheres(), m_tris(), m_intersections(0), m_hits(0)
{
    //ctor
    Material mat;

    mat.color = Color<>(0.4, 0.3, 0.3);
    add(gmtl::Plane<FLOAT>(gmtl::Vec<FLOAT,3>(0,1,0), -4.4), mat);

    mat.reflect = 0.6;
    mat.color = Color<>(0.7,0.7,0.7);
    add(gmtl::Sphere<FLOAT>(gmtl::Point<FLOAT,3>(1,-0.8,3), 2.5), mat);

    mat.reflect = 0.9;
    mat.color = Color<>(0.7,0.7,1);
	add(gmtl::Sphere<FLOAT>(gmtl::Point<FLOAT,3>(-5.5,-0.5,7), 2), mat);

    mat.emit = true;
    mat.color = Color<>(0.6,0.6,0.6);
    mat.reflect = 0;
    add(gmtl::Sphere<FLOAT>(gmtl::Point<FLOAT,3>(0,5,5), 0.1), mat);

    mat.color = Color<>(0.7,0.7,0.9);
    add(gmtl::Sphere<FLOAT>(gmtl::Point<FLOAT,3>(2,5,1), 0.1), mat);
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
        FLOAT t0 = 0, t1 = 0;
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

        for(size_t i = 0; i < m_spheres.size(); i++)
        {
            if(m_spheres[i].second.emit)
            {
                gmtl::Vec<FLOAT, 3> L = m_spheres[i].first.getCenter() - p;
                gmtl::normalize(L);
                gmtl::Ray<FLOAT> ray(p+static_cast<FLOAT>(0.001)*L, L);
                if(intersect(ray, type, id) < HUGE_VAL && type == SPHERE && id == i)
                {
                    c.add(diffuse(L, n, m_spheres[i].second, mat));
                    c.add(specular(L, n, r.getDir(), m_spheres[i].second, mat));
                }
            }
        }

        for(size_t i = 0; i < m_tris.size(); i++)
        {
            if(m_tris[i].second.emit)
            {
                gmtl::Vec<FLOAT, 3> L = gmtl::center(m_tris[i].first) - p;
                gmtl::normalize(L);
                gmtl::Ray<FLOAT> ray(p+static_cast<FLOAT>(0.001)*L, L);
                if(intersect(ray, type, id) < HUGE_VAL && type == SPHERE && id == i)
                {
                    c.add(diffuse(L, n, m_tris[i].second, mat));
                    c.add(specular(L, n, r.getDir(), m_tris[i].second, mat));
                }
            }
        }

        if(mat.reflect > 0)
        {
            gmtl::Vec<FLOAT, 3> R;
            gmtl::reflect(R, r.getDir(), n);
            gmtl::Ray<FLOAT> ray(p+static_cast<FLOAT>(0.001)*R, R);
            Color<> ref = radiance(ray, depth+1);
            ref.mult(mat.color);
            ref.mult(mat.reflect);
            c.add(ref);
        }
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
