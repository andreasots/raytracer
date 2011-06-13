#include "vector.h"
#include "exception.h"
#include <cmath>
#include <iostream> 
#define SQ(x) ((x)*(x))

namespace Raytracer
{
    Vector::Vector(float x, float y, float z): Point(x, y, z)
    {
        this->mW = 0;
    }

    Vector Vector::operator-() const
    {
        Vector ret = *this;
        ret.mX = -ret.mX;
        ret.mY = -ret.mY;
        ret.mZ = -ret.mZ;
        return ret;
    }

    Vector Vector::operator+(const Vector v) const
    {
        Vector ret = *this;
        ret += v;
        return ret;
    }

    Vector Vector::operator-(const Vector v) const
    {
        Vector ret = *this;
        ret += -v;
        return ret;
    }

    Vector Vector::operator*(const float i) const
    {
        Vector ret = *this;
        ret *= i;
        return ret;
    }

    VecMult Vector::operator*(const Vector u) const
    {
        return VecMult(*this, u);
    }

    Vector Vector::operator/(const float i) const
    {
        Vector ret = *this;
        ret *= 1/i;
        return ret;
    }

    Vector &Vector::operator+=(const Vector v)
    {
        this->mX += v.mX;
        this->mY += v.mY;
        this->mZ += v.mZ;
        return *this;
    }

    Vector &Vector::operator-=(const Vector v)
    {
        *this += -v;
        return *this;
    }

    Vector &Vector::operator*=(const float i)
    {
        this->mX *= i;
        this->mY *= i;
        this->mZ *= i;
        return *this;
    }

    Vector &Vector::operator/=(const float i)
    {
        *this *= 1/i;
        return *this;
    }

    bool Vector::operator==(const Vector v) const
    {
        return SQ(this->mX)+SQ(this->mY)+SQ(this->mZ) ==
            SQ(v.mX)+SQ(v.mY)+SQ(v.mZ);
    }

    bool Vector::operator<(const Vector v) const
    {
        return SQ(this->mX)+SQ(this->mY)+SQ(this->mZ) <
            SQ(v.mX)+SQ(v.mY)+SQ(v.mZ);
    }

    VecMult::operator float() const
    {
        float ret = this->mV.mX*this->mU.mX + this->mV.mY*this->mU.mY;
        ret += this->mV.mZ*this->mU.mZ + this->mV.mW*this->mU.mW;
        return ret;
    }

    VecMult::operator Vector() const
    {
        float x, y, z;
        x = this->mV.mY*this->mU.mZ-this->mV.mZ*this->mU.mY;
        y = this->mV.mX*this->mU.mZ-this->mV.mZ*this->mU.mX;
        z = this->mV.mX*this->mU.mY-this->mV.mY*this->mU.mX;
        std::cout << x << " " << y << " " << z << std::endl;
        return Vector(x, -y, z);
    }

    VecMult::VecMult(): mV(0, 0, 0), mU(0, 0, 0)
    {
        throw Exception("This constructor must never be called");
    }

    VecMult::VecMult(Vector v, Vector u): mV(v), mU(u)
    {
    }
}
