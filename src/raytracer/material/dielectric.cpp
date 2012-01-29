#include "raytracer/material/dielectric.h"
#include "raytracer/color.h"
#include "raytracer/scene.h"
namespace Raytracer {
namespace material {

Dielectric::Dielectric(RT_FLOAT n): m_n(n)
{
    //ctor
}

Dielectric::~Dielectric()
{
    //dtor
}

Color Dielectric::color(const SIMD::Point &p, const SIMD::Matrix &m, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const
{
    Color ret;
    SIMD::Vec d, N = m[2];
    RT_FLOAT n1 = 1, n2 = m_n, n1n2 = n1/n2;

    RT_FLOAT cos_theta_1 = -N.dot(in);
    if(cos_theta_1 < 0)
    {
        N = -N;
        cos_theta_1 = -cos_theta_1;
        n1 = m_n;
        n2 = 1;
        n1n2 = n1/n2;
    }

    RT_FLOAT cos_theta_2 = 1-n1n2*n1n2*(1-cos_theta_1*cos_theta_1);
    if(cos_theta_2 < 0)
    {
        ret.add(scene.radiance(SIMD::Ray(p, in+2*cos_theta_1*N), depth+1, dsfmt));
        return ret;
    }
    cos_theta_2 = std::sqrt(cos_theta_2);

    RT_FLOAT Rs = (n1*cos_theta_1-n2*cos_theta_2)/(n1*cos_theta_1+n2*cos_theta_2);
    Rs *= Rs;
    RT_FLOAT Rp = (n1*cos_theta_2-n2*cos_theta_1)/(n1*cos_theta_2+n2*cos_theta_1);
    Rp *= Rp;
    RT_FLOAT R = (Rp+Rs)*0.5;

    if(R > dsfmt_genrand_close_open(&dsfmt))
        d = in + 2*cos_theta_1*N;
    else
        d = n1n2*in+(n1n2*cos_theta_1-cos_theta_2)*N;

    ret.add(scene.radiance(SIMD::Ray(p, d), depth+1, dsfmt));

    return ret;
}

} // namespace Material
} // namespace Raytracer
