#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H
#include <algorithm>
#include <PixelToaster.h>
#include <xmmintrin.h>

namespace Raytracer
{
// a class for sRGB color
class Color
{
    __m128 m_data;
public:
    /** Constructor
     * \param r Red component
     * \param g Green component
     * \param b Blue component
     */
    Color(float r, float g, float b): m_data(_mm_set_ps(r, g, b, 0))
    {
    }

    Color(float val): m_data(_mm_set_ps(val, val, val, 0))
    {
    }

    Color(PixelToaster::Pixel p): m_data(_mm_set_ps(p.r, p.g, p.b, p.a))
    {
    }

    /** Default constructor */
    Color(): m_data(_mm_setzero_ps())
    {
    }

    /** Get red componet
     * \return The red component
     */
    float r() const
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(m_data, m_data, _MM_SHUFFLE(3, 3, 3, 3)));
    }

    /** Access green component
     * \return The green component
     */
    float g() const
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(m_data, m_data, _MM_SHUFFLE(2, 2, 2, 2)));
    }

    /** Access blue component
     * \return The blue component
     */
    float b() const
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(m_data, m_data, _MM_SHUFFLE(1, 1, 1, 1)));
    }

    /** Do sRGB gamma correction */
    void gamma()
    {
        float m_r = r(), m_g = g(), m_b = b();
        if(m_r <= 0.0031308)
            m_r *= 12.92;
        else
            m_r = 1.055*std::pow(m_r, 1/2.4) - 0.055;

        if(m_g <= 0.0031308)
            m_g *= 12.92;
        else
            m_g = 1.055*std::pow(m_g, 1/2.4) - 0.055;

        if(m_b <= 0.0031308)
            m_b *= 12.92;
        else
            m_b = 1.055*std::pow(m_b, 1/2.4) - 0.055;
        m_data = _mm_set_ps(m_r, m_g, m_b, 0);
    }

    /** Do gamma correction
     * \param Gamma The gamma value
     */
    void gamma(float Gamma)
    {
        float m_r = r(), m_g = g(), m_b = b();
        m_r = std::pow(m_r, Gamma);
        m_g = std::pow(m_g, Gamma);
        m_b = std::pow(m_b, Gamma);
        m_data = _mm_set_ps(m_r, m_g, m_b, 0);
    }

    Color &add(Color c)
    {
        m_data = _mm_add_ps(m_data, c.m_data);
        return *this;
    }
    Color &sub(Color c)
    {
        m_data = _mm_sub_ps(m_data, c.m_data);
        return *this;
    }

    Color &mult(float x)
    {
        m_data = _mm_mul_ps(m_data, _mm_set1_ps(x));
        return *this;
    }

    Color &div(float x)
    {
        m_data = _mm_div_ps(m_data, _mm_set1_ps(x));
    }

    Color &mult(Color c)
    {
        m_data = _mm_mul_ps(m_data, c.m_data);
        return *this;
    }

    PixelToaster::Pixel PT() const
    {
        return PixelToaster::Pixel(r(), g(), b());
    }

    float luminance() const
    {
        return 0.2126 * r() + 0.7152 * g() + 0.0722 * b();
        // says the Wikipedia
    }
};

} // namespace raytracer

#endif // RAYTRACER_COLOR_H
