#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H
#include <cmath>
#include <algorithm>

namespace Raytracer
{
template<class T = FLOAT>
class Color
{
public:
    /** Constructor
     * \param r Red component
     * \param g Green component
     * \param b Blue component
     */
    Color(T _r, T _g, T _b): m_r(_r), m_g(_g), m_b(_b)
    {
    }

    /** Default constructor */
    Color(): m_r(0), m_g(0), m_b(0)
    {
    }

    /** Get red componet
     * \return The red component
     */
    T r()
    {
        return m_r;
    }

    /** Set red component
     * \param val The red component
     */
    void r(T val)
    {
        m_r = val;
    }

    /** Access green component
     * \return The green component
     */
    T g()
    {
        return m_g;
    }

    /** Set green component
     * \param val The green component
     */
    void g(T val)
    {
        m_g = val;
    }

    /** Access blue component
     * \return The blue component
     */
    T b()
    {
        return m_b;
    }

    /** Set blue component
     * \param val The blue component
     */
    void b(T val)
    {
        m_b = val;
    }

    /** Do gamma correction
     * \param Gamma The gamma value
     */
    void gamma(T Gamma = 1/2.2)
    {
        m_r = std::pow(m_r, Gamma);
        m_g = std::pow(m_g, Gamma);
        m_b = std::pow(m_b, Gamma);
    }

    /** Clamp the values */
    void clamp()
    {
        m_r = std::max<T>(0, std::min<T>(m_r, 1));
        m_g = std::max<T>(0, std::min<T>(m_g, 1));
        m_b = std::max<T>(0, std::min<T>(m_b, 1));
    }

    Color<T> &add(Color c)
    {
        m_r += c.r();
        m_g += c.g();
        m_b += c.b();
        return *this;
    }

    Color<T> &mult(T x)
    {
        m_r *= x;
        m_g *= x;
        m_b *= x;
        return *this;
    }

    Color<T> mult(Color<T> c)
    {
        m_r *= c.r();
        m_g *= c.g();
        m_b *= c.b();
        return *this;
    }

protected:
private:
    T m_r; //!< The red component
    T m_g; //!< The green component
    T m_b; //!< The blue component
};

} // namespace raytracer

#endif // RAYTRACER_COLOR_H
