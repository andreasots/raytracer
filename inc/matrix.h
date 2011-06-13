#ifndef RAYTRACER_MATRIX_H
#define RAYTRACER_MATRIX_H
#include <vector>
namespace Raytracer
{
    class Matrix
    {
        public:
            Matrix(unsigned w, unsigned h);
            Matrix();
            static Matrix identity(unsigned n);
            static Matrix identity();
            float get(unsigned x, unsigned y);
            void set(unsigned x, unsigned y, float n);
            float operator()(unsigned x, unsigned y);
            Matrix operator+(Matrix m) const;
            Matrix operator-(Matrix m) const;
            Matrix operator*(Matrix m) const;
            Matrix operator*(float x) const;
            Matrix operator/(float x) const;
            Matrix &operator+=(Matrix m);
            Matrix &operator-=(Matrix m);
            Matrix &operator*=(Matrix m);
            Matrix &operator*=(float x);
            Matrix &operator/=(float x);
        protected:
            std::vector<float> mMatrix;
            unsigned mW, mH;
    };
}
#endif