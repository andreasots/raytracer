#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H
#include <cmath>
#include <algorithm>

namespace Raytracer
{
template<class T = float>
class Color
{
public:
    /** Constructor
     * \param r Red component
     * \param g Green component
     * \param b Blue component
     */
    Color(T r, T g, T b): m_r(r), m_g(g), m_b(b)
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
        m_r = std::max(0, std::min(m_r, 1));
        m_g = std::max(0, std::min(m_g, 1));
        m_b = std::max(0, std::min(m_b, 1));
    }

    Color<T> &add(Color c)
    {
        m_r += c.r();
        m_g += c.g();
        m_b += c.b();
    }

    Color<T> mult(T x)
    {
        Color<T> ret = *this;
        ret.m_r *= x;
        ret.m_g *= x;
        ret.m_b *= x;
        return ret;
    }

protected:
private:
    T m_r; //!< The red component
    T m_g; //!< The green component
    T m_b; //!< The blue component
};

} // namespace raytracer

#endif // RAYTRACER_COLOR_H
