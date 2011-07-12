#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

#include <stdexcept>
#include <cmath>

namespace Raytracer
{
    template<class T = float, size_t N = 3>
    class Vector
    {
        public:
        Vector(std::initializer_list<T> list) throw()
        {
            if(list.size() < N)
            {
                size_t i;
                for(i = 0; i < list.size(); i++)
                    m_vector[i] = *(list.begin()+i);
                for(; i < N; i++)
                    m_vector[i] = 0;
            }
            else
                for(size_t i = 0; i < N; i++)
                    m_vector[i] = *(list.begin()+i);
        }

        Vector(const Vector<T, N> &vec) throw()
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] = vec.m_vector[i];
        }

        Vector<T, N>& operator=(const Vector<T, N> &vec) throw()
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] = vec.m_vector[i];
            return *this;
        }

        Vector() throw()
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] = 0;
        }

        Vector<T, N+1> homogeneous() throw()
        {
            Vector<T, N+1> ret;
            for(size_t i = 0; i < N; i++)
                ret[i] = m_vector[i];
            ret[N] = 1;
            return ret;
        }

        Vector<T, N-1> project() throw()
        {
            Vector<T, N-1> ret;
            for(size_t i = 0; i < N-1; i++)
                ret[i] = m_vector[i]/m_vector[N-1];
            return ret;
        }

        Vector<T, N>& add(Vector<T, N> vec) throw()
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] += vec[i];
            return *this;
        }

        Vector<T, N> cross(Vector<T, N> vec) throw(std::domain_error)
        {
            if(N != 3)
                throw std::domain_error("Vector::cross");
            Vector<T, N> ret;
            ret[0] = m_vector[1] * vec[2] - m_vector[2] * vec[1];
            ret[1] = m_vector[2] * vec[0] - m_vector[0] * vec[2];
            ret[2] = m_vector[0] * vec[1] - m_vector[1] * vec[0];
            return ret;
        }

        T dot(Vector<T, N> vec) throw()
        {
            T ret;
            for(size_t i = 0; i < N; i++)
                ret += m_vector[i] * vec[i];
            return ret;
        }

        Vector<T, N>& mult(T x) throw()
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] *= x;
            return *this;
        }

        Vector<T, N>& div(T x) throw()
        {
            return this->mult(1/x);
        }

        Vector<T, N>& normalise() throw()
        {
            T length = this->dot(*this);
            if(length != 1 || length != 0)
                this->div(std::sqrt(length));
            return *this;
        }

        T& at(size_t i) throw(std::out_of_range)
        {
            if(i >= N)
                throw std::out_of_range("Vector::at");
            return m_vector[i];
        }

        T& operator[](size_t i) throw(std::out_of_range)
        {
            return this->at(i);
        }

        T length() throw()
        {
            return std::sqrt(this->dot(*this));
        }

        Vector<T, N> operator+(const Vector<T, N> v) const throw()
        {
            Vector<T, N> ret = *this;
            ret.add(v);
            return ret;
        }

        Vector<T, N> operator-(const Vector<T, N> v) const throw()
        {
            Vector<T, N> ret = *this;
            ret.add(-v);
            return ret;
        }

        Vector<T, N> operator-() const throw()
        {
            Vector<T, N> ret;
            for(size_t i; i < N; i++)
                ret[i] = -m_vector[i];
            return ret;
        }

        protected:
        T m_vector[N];
};

} // namespace Raytracer

#endif // RAYTRACER_VECTOR_H
