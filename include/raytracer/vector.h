#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

namespace Raytracer
{
class Mult
{
public:
    Mult(Vector v, Vector u)
};

class Vector
{
public:
    Vector(float x, float y, float z);
    float x()
    {
        return m_x;
    }
    void x(float val)
    {
        m_x = val;
    }
    float y()
    {
        return m_y;
    }
    void y(float val)
    {
        m_y = val;
    }
    float z()
    {
        return m_z;
    }
    void z(float val)
    {
        m_z = val;
    }
    float w()
    {
        return m_w;
    }
    void w(float val)
    {
        m_w = val;
    }
protected:
private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};

} // namespace Raytracer

#endif // RAYTRACER_VECTOR_H
