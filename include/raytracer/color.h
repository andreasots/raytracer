#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H
#include <cmath>

namespace raytracer
{

class Color
{
public:
    /** Constructor
     * \param r Red component
     * \param g Green component
     * \param b Blue component
     */
    Color(float r, float g, float b): m_r(r), m_g(g), m_b(b)
    {
    }

    /** Default constructor */
    Color(): m_r(0), m_g(0), m_b(0)
    {
    }

    /** Get red componet
     * \return The red component
     */
    float r()
    {
        return m_r;
    }

    /** Set red component
     * \param val The red component
     */
    void r(float val)
    {
        m_r = val;
    }

    /** Access green component
     * \return The green component
     */
    float g()
    {
        return m_g;
    }

    /** Set green component
     * \param val The green component
     */
    void g(float val)
    {
        m_g = val;
    }

    /** Access blue component
     * \return The blue component
     */
    float b()
    {
        return m_b;
    }

    /** Set blue component
     * \param val The blue component
     */
    void b(float val)
    {
        m_b = val;
    }

    /** Do gamma correction */
    void gamma()
    {
        gamma(1/2.2);
    }

    /** Do gamma correction
     * \param Gamma The gamma value
     */
    void gamma(float Gamma)
    {
        r = std::pow(r, Gamma);
        g = std::pow(g, Gamma);
        b = std::pow(b, Gamma);
    }

    /** Clamp the values */
    void clamp()
    {
        r = max(0.0f, min(r, 1.0f));
        g = max(0.0f, min(g, 1.0f));
        b = max(0.0f, min(b, 1.0f));
    }

protected:
private:
    float m_r; //!< The red component
    float m_g; //!< The green component
    float m_b; //!< The blue component
};

} // namespace raytracer

#endif // RAYTRACER_COLOR_H
