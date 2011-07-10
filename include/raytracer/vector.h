#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

namespace Raytracer
{
    template<class T = float, size_t N = 3>
    class Vector
    {
        public:
        Vector(std::initializer_list<T> list)
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

        Vector(Vector<T, N> &vec)
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] = vec[i];
        }

        Vector()
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] = 0;
        }

        Vector<T, N+1> homogeneous()
        {
            Vector<T, N+1> ret;
            for(size_t i = 0; i < N; i++)
                ret[i] = m_vector[i];
            ret[N] = 1;
            return ret;
        }

        Vector<T, N-1> project()
        {
            Vector<T, N-1> ret;
            for(size_t i = 0; i < N-1; i++)
                ret[i] = m_vector[i]/m_vector[N-1];
            return ret;
        }

        Vector<T, N>& add(Vector<T, N> vec)
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] += vec[i];
            return *this;
        }

        Vector<T, N> cross(Vector<T, N> vec)
        {
            throw Exception("Vector::cross: Not defined");
        }

        T dot(Vector<T, N> vec)
        {
            T ret;
            for(size_t i = 0; i < N; i++)
                ret += m_vector * vec[i];
            return ret;
        }

        Vector<T, N>& div(T x)
        {
            for(size_t i = 0; i < N; i++)
                m_vector[i] /= x;
            return *this;
        }

        Vector<T, N>& normalise()
        {
            T length = this->dot(*this);
            if(length != 1)
                this->div(sqrt(length));
            return *this;
        }

        protected:
        T m_vector[N];
};

} // namespace Raytracer

#endif // RAYTRACER_VECTOR_H
