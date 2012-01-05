#include "raytracer/scene.h"

#include "raytracer/allocator.h"

#include "raytracer/sphere.h"
#include "raytracer/tri.h"
#include "raytracer/cylinder.h"

#include "raytracer/material/ashikhminshirley.h"
#include "raytracer/material/phong.h"
#include "raytracer/material/dielectric.h"
#include "raytracer/material/mirror.h"
#include "raytracer/material/null.h"

#include <SIMD/Vec.h>
#include <SIMD/Point.h>
#include <SIMD/AABox.h>
#include <SIMD/Matrix.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>

#define MAXDEPTH 256

template<class T> static std::string intToString(T i)
{
    if(i < 0)
        return "-"+intToString(-i);
    if(i < 10)
    {
        char buf[2];
        buf[0] = '0'+i;
        buf[1] = 0;
        return std::string(buf);
    }
    else
        return intToString(i/10)+intToString(i%10);
}

namespace Raytracer {

Scene::Scene(): m_objects(), m_intersections(0), m_hits(0), m_sky(0, 0, 0)
{
}

Scene::~Scene()
{
    while(!m_objects.empty())
    {
        deallocate<Object, 16>(m_objects.back(), 1);
        m_objects.pop_back();
    }
}

SIMD::Matrix Scene::open(std::string file)
{
    std::ifstream in(file.c_str());
    in.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
    std::map<std::string, Material*> materials;
    SIMD::Matrix camera = SIMD::Matrix::identity();
    unsigned long statement = 0;
    try
    {
        while(true)
        {
            std::string token;
            in >> token;
            statement++;
            if(token == "material")
            {
                token = "";
                char c;
                in.ignore(1024, '"');
                in.get(c);
                while(c != '"')
                {
                    token += c;
                    in.get(c);
                }
                Material *mat;
                std::string name = token;
                in >> token;
                if(token == "Phong")
                {
                    RT_FLOAT diff, spec, spec_pow, emit, r, g, b;
                    in >> diff >> r >> g >> b;
                    Color c_diff(r, g, b);
                    in >> spec >> spec_pow >> r >> g >> b;
                    Color c_spec(r, g, b);
                    in >> emit >> r >> g >> b;
                    Color c_emit(r, g, b);
                    mat = new (allocate<material::Phong, 16>(1)) material::Phong(diff, c_diff, spec, spec_pow, c_spec, emit, c_emit);
                }
                else if(token == "Null")
                {
                    RT_FLOAT r, g, b;
                    in >> r >> g >> b;
                    mat = new (allocate<material::Null, 16>(1)) material::Null(Color(r, g, b));
                }
                else if(token == "Dielectric")
                {
                    RT_FLOAT n;
                    in >> n;
                    mat = new (allocate<material::Dielectric, 16>(1)) material::Dielectric(n);
                }
                else if(token == "AshikhminShirley")
                {
                    RT_FLOAT nv, nu, r, g, b;
                    in >> r >> g >> b;
                    Color Rd(r, g, b);
                    in >> nu >> nv >> r >> g >> b;
                    Color Rs(r, g, b);
                    mat = new (allocate<material::AshikhminShirley, 16>(1)) material::AshikhminShirley(Rd, nu, nv, Rs);
                }
                else if(token == "Mirror")
                    mat = new material::Mirror();
                else
                    std::runtime_error("Unrecognised token '"+token+"' after 'material \""+name+"\"' in statement "+intToString(statement));
                if(materials.count(name))
                    delete materials[name];
                materials[name] = mat;
            }
            else if(token == "camera")
            {
                in >> token;
                if(token == "matrix")
                {
                    RT_FLOAT mat[16];
                    for(int i = 0; i < 16; i++)
                        in >> mat[i];
                    camera = SIMD::Matrix(mat);
                }
                else if(token == "lookat")
                {
                    // http://www.opengl.org/sdk/docs/man/xhtml/gluLookAt.xml
                    RT_FLOAT x, y, z;
                    in >> x >> y >> z;
                    SIMD::Point eye(x, y, z);
                    in >> x >> y >> z;
                    SIMD::Point center(x, y, z);
                    in >> x >> y >> z;
                    SIMD::Vec up(x, y, z);

                    SIMD::Vec f = center-eye;
                    f.normalize();
                    up.normalize();
                    SIMD::Vec s = f.cross(up);
                    SIMD::Vec u = s.cross(f);
                    f = -f;
                    camera = SIMD::Matrix(s.data(), u.data(), f.data(), eye.data());
                }
                else
                    throw std::runtime_error("Unrecognised token '"+token+"' after 'camera' in statement "+intToString(statement));
            }
            else if(token == "triangle")
            {
                token = "";
                char c;
                in.ignore(1024, '"');
                in.get(c);
                while(c != '"')
                {
                    token += c;
                    in.get(c);
                }

                RT_FLOAT x, y, z;
                in >> x >> y >> z;
                SIMD::Point A(x, y, z);
                in >> x >> y >> z;
                SIMD::Point B(x, y, z);
                in >> x >> y >> z;
                SIMD::Point C(x, y, z);
                if(!materials.count(token))
                    materials[token] = new (allocate<material::Null, 16>(1)) material::Null(Color());
                Tri *t = new (allocate<Tri, 16>(1)) Tri(A, B, C, materials[token]);
                in >> token;
                if(token == "normals")
                {
                    in >> x >> y >> z;
                    SIMD::Vec n0(x, y, z);
                    in >> x >> y >> z;
                    SIMD::Vec n1(x, y, z);
                    in >> x >> y >> z;
                    SIMD::Vec n2(x, y, z);
                    t->normals(n0, n1, n2);
                }
                else if(token != "flat")
                {
                    throw std::runtime_error("Unrecognised token '"+token+"' after 'triangle' in statement "+intToString(statement));
                }
                add(t);
            }
            else if(token == "sphere")
            {
                token = "";
                char c;
                in.ignore(1024, '"');
                in.get(c);
                while(c != '"')
                {
                    token += c;
                    in.get(c);
                }

                RT_FLOAT x, y, z;
                in >> x >> y >> z;
                SIMD::Point O(x, y, z);
                in >> x;
                if(!materials.count(token))
                    materials[token] = new (allocate<material::Null, 16>(1)) material::Null(Color());
                add(new (allocate<Sphere, 16>(1)) Sphere(O, x, materials[token]));
            }
            else if(token == "cylinder")
            {
                token = "";
                char c;
                in.ignore(1024, '"');
                in.get(c);
                while(c != '"')
                {
                    token += c;
                    in.get(c);
                }

                RT_FLOAT x, y, z;
                in >> x >> y >> z;
                SIMD::Point A(x, y, z);
                in >> x >> y >> z;
                SIMD::Point B(x, y, z);
                in >> x;
                if(!materials.count(token))
                    materials[token] = new (allocate<material::Null, 16>(1)) material::Null(Color());
                add(new (allocate<Cylinder, 16>(1)) Cylinder(A, B, x, materials[token]));
            }
            else if(token == "sky")
            {
                RT_FLOAT r, g, b;
                in >> r >> g >> b;
                m_sky = Color(r, g, b);
            }
            else
                throw std::runtime_error("Unrecognised token '"+token+"' in statement "+intToString(statement));
        }
    }
    catch(std::exception &e)
    {
        std::cerr << file << ": reading stopped: " << e.what() << std::endl;
    }

    return camera;
}

void Scene::add(Object *o)
{
    m_objects.push_back(o);
}

RT_FLOAT Scene::intersect(const SIMD::Ray &r, size_t &id, RT_FLOAT &u, RT_FLOAT &v, RT_FLOAT max)
{
    RT_FLOAT ret = max;

    for(size_t i = 0; i < m_objects.size(); i++)
    {
        RT_FLOAT U, V;
        RT_FLOAT T = m_objects[i]->intersect(r, U, V);
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

Color Scene::radiance(const SIMD::Ray &r, size_t depth, dsfmt_t &dsfmt)
{
    Color c;
    // SIGSEGV protection
    if(depth > MAXDEPTH)
        return c;
    size_t id;
    RT_FLOAT u, v;
    RT_FLOAT t = this->intersect(r, id, u, v);

    if(t < HUGE_VAL)
    {
        SIMD::Point p = r.origin+t*r.direction;
        Object *o = m_objects[id];
        const Material *mat = o->material();
        SIMD::Vec n = o->tangentSpace(u, v)[2];

        c.add(mat->color(p, n, r.direction, *this, depth, dsfmt));
    }
    else
        c = m_sky;

    return c;
}

void Scene::stats()
{
    std::cout << "\nSTATS" << std::endl;
    std::cout << "Number of primitives: " << m_objects.size() << std::endl;
    std::cout << "Number of intersection tests: " << m_intersections << std::endl;
    std::cout << "Number of successful intersection tests: " << m_hits << std::endl;
}

} // namespace Raytracer
