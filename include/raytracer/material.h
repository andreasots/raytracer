#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "raytracer/color.h"

#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

namespace Raytracer
{
    struct Material
    {
        RT_FLOAT diffuse, specular, spec_pow;
        Color<> color;
        bool emit;
    private:
        RT_FLOAT spec_weight;

    public:
        Material(): specular(0.5), diffuse(0.5), spec_pow(20), color(), emit(false)
        {
            spec_weight = (spec_pow+2)*M_1_PI*0.5;
        }

        Color<> diffuseCalc(const Color<> &lambda) const
        {
            Color<> ret = lambda;
            ret.mult(M_1_PI);
            ret.mult(color);
            return ret;
        }

        Color<> specularCalc(const Color<> &lambda) const
        {
            Color<> ret = lambda;
            ret.mult(spec_weight);
            return ret;
        }

        gmtl::Vec<RT_FLOAT, 3> diffuseSample(const gmtl::Vec<RT_FLOAT, 3> &n) const
        {
            // Cosine weighted random hemisphere sampling
            // from smallpt by Kevin Beason
            RT_FLOAT r1=2*M_PI*drand48(), r2=drand48(), r2s=std::sqrt(r2);

            gmtl::Vec<RT_FLOAT, 3> u, v;
            if(std::abs(n[0]) > 0.1)
                gmtl::cross(u, gmtl::Vec<RT_FLOAT, 3>(0,1,0), n);
            else
                gmtl::cross(u, gmtl::Vec<RT_FLOAT, 3>(1,0,0), n);
            gmtl::normalize(u);
            gmtl::cross(v, n, u);

            gmtl::Vec<RT_FLOAT, 3> d = u*std::cos(r1)*r2s + v*std::sin(r1)*r2s + n*std::sqrt(1-r2);
            gmtl::normalize(d);

            return d;
        }

        gmtl::Vec<RT_FLOAT, 3> specularSample(const gmtl::Vec<RT_FLOAT, 3> &n, const gmtl::Vec<RT_FLOAT, 3> &in) const
        {
            gmtl::Vec<RT_FLOAT, 3> x, y, z, ret;
            gmtl::reflect(z, in, n);

            if(std::abs(z[0]) > 0.1)
                gmtl::cross(x, gmtl::Vec<RT_FLOAT, 3>(0,1,0), z);
            else
                gmtl::cross(x, gmtl::Vec<RT_FLOAT, 3>(1,0,0), z);
            gmtl::normalize(x);
            gmtl::cross(y, z, x);

            do
            {
                RT_FLOAT cos_theta = std::pow(drand48(), 1/(spec_pow+2)), phi = 2*M_PI*drand48();
                RT_FLOAT sin_theta = std::sqrt(1-cos_theta*cos_theta);

                ret = x*std::cos(phi)*sin_theta+y*std::sin(phi)*sin_theta+z*cos_theta;
            }
            while(gmtl::dot(ret, n) < 0);

            gmtl::normalize(ret);

            return ret;
        }
    };
}

#endif
