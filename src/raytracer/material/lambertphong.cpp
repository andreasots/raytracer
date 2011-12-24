#include "raytracer/material/lambertphong.h"
#include "raytracer/scene.h"

namespace Raytracer {
namespace material {

LambertPhong::LambertPhong(RT_FLOAT _diffuse, RT_FLOAT _specular, RT_FLOAT _spec_pow, bool _emit, Color _color):
diffuse(_diffuse), specular(_specular), spec_pow(_spec_pow), emit(_emit), m_color(_color)
{
    //ctor
}

LambertPhong::~LambertPhong()
{
    //dtor
}

Color LambertPhong::color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth) const
{
    Color ret;
    if(emit)
        ret.add(m_color);
    if(n.dot(in) > 0)
        return ret;
    RT_FLOAT u = drand48();
    if(diffuse > u)
    {
        // Cosine weighted random hemisphere sampling
        // from smallpt by Kevin Beason
        RT_FLOAT r1=2*M_PI*drand48(), r2=drand48(), r2s=std::sqrt(r2);

        SIMD::Vec u, v;
        if(std::abs(n[0]) > 0.1)
            u = SIMD::Vec(0, 1, 0).cross(n);
        else
            u = SIMD::Vec(1, 0, 0).cross(n);
        u.normalize();
        v = n.cross(u);

        SIMD::Vec d = u*std::cos(r1)*r2s + v*std::sin(r1)*r2s + n*std::sqrt(1-r2);
        d.normalize();
        // End of CWRHS

        Raytracer::Color lambda = scene.radiance(SIMD::Ray(p, d), depth+1);
        lambda.mult(m_color);
        ret.add(lambda);
    }
    else if(diffuse+specular > u)
    {
        // Importance sampling of Phong reflection model by Jason Lawrence
        SIMD::Vec u, v, out = in-2*in.dot(n)*n, d;

        if(std::abs(out[0]) > 0.1)
            u = SIMD::Vec(0, 1, 0).cross(out);
        else
            u = SIMD::Vec(1, 0, 0).cross(out);
        u.normalize();
        v = out.cross(u);

        do
        {
            RT_FLOAT cos_theta = std::pow(drand48(), 1/(spec_pow+2)), phi = 2*M_PI*drand48();
            RT_FLOAT sin_theta = std::sqrt(1-cos_theta*cos_theta);

            d = u*std::cos(phi)*sin_theta+v*std::sin(phi)*sin_theta+out*cos_theta;
        }
        while(d.dot(n) < 0);
        d.normalize();
        // End of ISoPRM

        ret.add(scene.radiance(SIMD::Ray(p, d), depth+1));
    }
    return ret;
}

} // namespace Material
} // namespace Raytracer
