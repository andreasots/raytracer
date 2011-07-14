#include "raytracer/scene.h"
#include "raytracer/ray.h"
#include "raytracer/sphere.h"
#include "raytracer/plane.h"

#include <cmath>
#include <iostream>

inline float clamp(float val)
{
    val *= val;
    return val > 1 ? 1 : (val < 0 ? 0 : val);
}

namespace Raytracer {

Scene::Scene(std::string file): m_intersections_total(0), m_intersections_successful(0)
{
    //ctor
    Material mat;
    mat.diffuse = Color<>(0, 1, 0);
    this->add(new Sphere({0, 0, 3}, mat, 2.5f));
}
void Scene::add(Object *o)
{
    this->m_objects.push_back(o);
}

float Scene::intersect(Ray<> r, size_t &id)
{
    float ret = HUGE_VALF;
    for(size_t i = 0; i < this->m_objects.size(); i++)
    {
        float t = m_objects[i]->intersect(r);
        m_intersections_total++;
        if(t < ret)
        {
            ret = t;
            id = i;
        }
        if(t < HUGE_VALF)
            m_intersections_successful++;
    }
    return ret;
}

Color<> Scene::radiance(Ray<> r, size_t depth)
{
    size_t id;
    float t = this->intersect(r, id);
    if(t < HUGE_VALF)
    {
        Color<> c = this->m_objects[id]->material().diffuse;
        return c;
        Vector<> pos = r.origin() + r.direction().mult(t), light = {0,5,5};
        light.normalise();
        return c.mult(clamp(this->m_objects[id]->normal(pos).dot(light)));
    }
    else
        return Color<>();
}

void Scene::stats()
{
    std::cout << "\nSTATS" << std::endl;
    std::cout << "Number of primitives: " << m_objects.size() << std::endl;
    std::cout << "Number of intersection tests: " << m_intersections_total << std::endl;
    std::cout << "Number of successful intersection tests: " << m_intersections_successful << std::endl;
}
} // namespace Raytracer
