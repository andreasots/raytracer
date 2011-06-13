#include "exception.h"
#include "matrix.h"

namespace Raytracer
{
    Matrix::Matrix(unsigned w, unsigned h): mMatrix(), mW(w), mH(h)
    {
        this->mMatrix.resize(w*h, 0);
    }
    
    Matrix::Matrix(): mMatrix(), mW(4), mH(4)
    {
        this->mMatrix.resize(16, 0);
    }

    Matrix Matrix::identity(unsigned n)
    {
        Matrix ret(n, n);
        for(unsigned i = 0; i < n; i++)
            ret.set(i, i, 1.0);
        return ret;
    }

    Matrix Matrix::identity()
    {
        return Matrix::identity(4);
    }

    float Matrix::get(unsigned x, unsigned y)
    {
        if(x < this->mW && y < this->mH)
          return this->mMatrix[y*this->mW+x];
        else
            throw Exception("Matrix::get(): Out of bounds");
    }

    void Matrix::set(unsigned x, unsigned y, float n)
    {
        if(x < this->mW && y < this->mH)
            this->mMatrix[y*this->mW+x] = n;
        else
            throw Exception("Matrix::set(): Out of bounds");
    }

    float Matrix::operator()(unsigned x, unsigned y)
    {
        return this->get(x, y);
    }

    Matrix Matrix::operator+(Matrix m) const
    {
        Matrix ret = *this;
        ret += m;
        return ret;
    }

    Matrix Matrix::operator-(Matrix m) const
    {
        Matrix ret = *this;
        ret -= m;
        return ret;
    }

    Matrix Matrix::operator*(Matrix m) const
    {
        Matrix ret = *this;
        ret *= m;
        return ret;
    }

    Matrix Matrix::operator*(float x) const
    {
        Matrix ret = *this;
        ret *= x;
        return ret;
    }

    Matrix Matrix::operator/(float x) const
    {
        Matrix ret = *this;
        ret /= x;
        return ret;
    }

    Matrix &Matrix::operator+=(Matrix m)
    {
        if(this->mW == m.mW && this->mH == m.mH)
        {
            for(unsigned i = 0; i < this->mH; i++)
            {
                for(unsigned j = 0; j < this->mW; j++)
                    this->set(j, i, this->get(j, i)+m.get(j, i));
            }
            return *this;
        }
        else
            throw Exception("Matrix::operator+=(): Not defined");
    }

    Matrix &Matrix::operator-=(Matrix m)
    {
        if(this->mW == m.mW && this->mH == m.mH)
        {
            for(unsigned i = 0; i < this->mH; i++)
            {
                for(unsigned j = 0; j < this->mW; j++)
                    this->set(j, i, this->get(j, i)-m.get(j, i));
            }
            return *this;
        }
        else
            throw Exception("Matrix::operator-=(): Not defined");
    }

    Matrix &Matrix::operator*=(Matrix m)
    {
        if(this->mH == m.mW)
        {
            Matrix *ret = new Matrix(this->mW, m.mH);
            for(unsigned y = 0; y < m.mH; y++)
            {
                for(unsigned x = 0; y < this->mW; y++)
                {
                    float val = 0;
                    for(unsigned i = 0; i < this->mH; i++)
                        val += this->get(x, i)*m.get(i, y);
                    ret->set(x, y, val);
                }
            }
            return *ret;
        }
        else
            throw Exception("Matrix::operator=*(): Not defined");
    }

    Matrix &Matrix::operator*=(float x)
    {
        for(unsigned i = 0; i < this->mH; i++)
            for(unsigned j = 0; j < this->mW; j++)
              this->set(j, i, this->get(j, i)*x);
        return *this;
    }

    Matrix &Matrix::operator/=(float x)
    {
        for(unsigned i = 0; i < this->mH; i++)
            for(unsigned j = 0; j < this->mW; j++)
              this->set(j, i, this->get(j, i)/x);
        return *this;
    }
}