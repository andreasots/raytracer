#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "raytracer/color.h"

#include <SIMD/Vec.h>

namespace Raytracer
{
    struct Material
    {
        RT_FLOAT diffuse, specular, spec_pow;
        Color<> color;
        bool emit;

    public:
        Material(): diffuse(0.5), specular(0.5), spec_pow(20), color(), emit(false)
        {
        }

        Material(RT_FLOAT diff, RT_FLOAT spec, RT_FLOAT specP, Color<> c, bool e):
            diffuse(diff), specular(spec), spec_pow(specP), color(c), emit(e)
        {
        }

        Color<> diffuseCalc(const Color<> &lambda) const
        {
            Color<> ret = lambda;
            ret.mult(color);
            return ret;
        }

        Color<> specularCalc(const Color<> &lambda) const
        {
            Color<> ret = lambda;
            return ret;
        }

        SIMD::Vec diffuseSample(const SIMD::Vec &n) const
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

            return d;
        }

        SIMD::Vec specularSample(const SIMD::Vec &n, const SIMD::Vec &in) const
        {
            SIMD::Vec x, y, z, ret;
            z = in-2*in.dot(n)*n;

            if(std::abs(z[0]) > 0.1)
                x = SIMD::Vec(0, 1, 0).cross(z);
            else
                x = SIMD::Vec(1, 0, 0).cross(z);
            x.normalize();
            y = z.cross(x);

            do
            {
                RT_FLOAT cos_theta = std::pow(drand48(), 1/(spec_pow+2)), phi = 2*M_PI*drand48();
                RT_FLOAT sin_theta = std::sqrt(1-cos_theta*cos_theta);

                ret = x*std::cos(phi)*sin_theta+y*std::sin(phi)*sin_theta+z*cos_theta;
            }
            while(ret.dot(n) < 0);

            ret.normalize();

            return ret;
        }
    };
}

#endif
