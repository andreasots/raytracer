#include "raytracer/material/ashikhminshirley.h"
#include "raytracer/scene.h"

namespace Raytracer {
namespace material {

AshikhminShirley::AshikhminShirley(Color _Rd, RT_FLOAT _nu, RT_FLOAT _nv, Color _Rs): Rd(_Rd), Rs(_Rs), nu(_nu), nv(_nv)
{
    //ctor
}

AshikhminShirley::~AshikhminShirley()
{
    //dtor
}

Color AshikhminShirley::color(const SIMD::Point &p, const SIMD::Vec &n, const SIMD::Vec &in, Scene &scene, size_t depth, dsfmt_t &dsfmt) const
{
    SIMD::Vec k2 = -in;
    Color ret;
    SIMD::Vec u, v;
    if(std::abs(n[0]) > 0.1)
        u = SIMD::Vec(0, 1, 0).cross(n);
    else
        u = SIMD::Vec(1, 0, 0).cross(n);
    u.normalize();
    v = n.cross(u);

    RT_FLOAT theta = 2*M_PI*dsfmt_genrand_close_open(&dsfmt), cos_phi = dsfmt_genrand_close_open(&dsfmt), sin_phi = std::sqrt(1-cos_phi*cos_phi);
    SIMD::Vec k1 = std::cos(theta)*sin_phi*u + std::sin(theta)*sin_phi*v + cos_phi*n;

    Color lambda = scene.radiance(SIMD::Ray(p, k1), depth+1, dsfmt);

    // Diffuse
    Color col = Rd;
    col.mult(28/(23*M_PI));
    Color tmp(1, 1, 1);
    tmp.sub(Rs);
    col.mult(tmp);
    col.mult(1-std::pow(1-n.dot(k1)/2, 5));
    col.mult(1-std::pow(1-n.dot(k2)/2, 5));
    col.mult(lambda);
    ret.add(col);

    // Specular
    SIMD::Vec h = k1+k2;
    h.normalize();
    col = lambda;
    col.mult(std::sqrt((nu+1)*(nv+1))/(8*M_PI));
    RT_FLOAT hk = h.dot(in), hu = h.dot(u), hv = h.dot(v), hn = h.dot(n);
    col.mult(std::pow(n.dot(h), (nu*std::pow(std::cos(theta), 2)+nv*std::pow(std::sin(theta), 2))));
    col.div(hk*std::max(n.dot(k1), n.dot(k2)));
    Color F = Rs;
    tmp.mult(std::pow(1-hk, 5));
    F.add(tmp);
    col.mult(F);
    ret.add(col);
    return Color(std::abs(ret.r()), std::abs(ret.g()), std::abs(ret.b()));
}

} // namespace material
} // namespace Raytracer
