#include "raytracer/material/phong.h"
#include "raytracer/scene.h"

namespace Raytracer {
namespace material {

Phong::Phong(RT_FLOAT _diffuse, Color c_diff, RT_FLOAT _specular, RT_FLOAT _spec_pow, Color c_spec, RT_FLOAT _emit, Color c_e):
diffuse(_diffuse), specular(_specular), spec_pow(_spec_pow), c_diffuse(c_diff), c_specular(c_spec), c_emit(c_e)
{
    c_emit.mult(_emit);
    //ctor
}

Phong::~Phong()
{
    //dtor
}

Color Phong::color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const
{
    Color ret;
    ret.add(c_emit);

    if(n.dot(in) > 0)
        return ret;

    RT_FLOAT xi = dsfmt_genrand_close_open(&dsfmt);
    if(diffuse > xi)
    {
        RT_FLOAT weight = std::max(c_diffuse.r(), std::max(c_diffuse.g(), c_diffuse.b()));
        Raytracer::Color lambda = c_diffuse;
        if(depth > 16)
            if(weight < dsfmt_genrand_close_open(&dsfmt))
                return ret;
            else // for conservation of energy:
                lambda.div(weight);

        // Cosine weighted random hemisphere sampling
        // from smallpt by Kevin Beason
        RT_FLOAT r1=2*M_PI*dsfmt_genrand_close_open(&dsfmt), r2=dsfmt_genrand_close_open(&dsfmt), r2s=std::sqrt(r2);

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

        lambda.mult(scene.radiance(SIMD::Ray(p, d), depth+1, dsfmt));
        ret.add(lambda);
    }
    else if(diffuse+specular > xi)
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
            RT_FLOAT cos_theta = std::pow(dsfmt_genrand_close_open(&dsfmt), 1/(spec_pow+2)), phi = 2*M_PI*dsfmt_genrand_close_open(&dsfmt);
            RT_FLOAT sin_theta = std::sqrt(1-cos_theta*cos_theta);

            d = u*std::cos(phi)*sin_theta+v*std::sin(phi)*sin_theta+out*cos_theta;
        }
        while(d.dot(n) < 0);
        d.normalize();
        // End of ISoPRM

        Color lambda = scene.radiance(SIMD::Ray(p, d), depth+1, dsfmt);
        lambda.mult(c_specular);
        ret.add(lambda);
    }
    return ret;
}

} // namespace Material
} // namespace Raytracer
