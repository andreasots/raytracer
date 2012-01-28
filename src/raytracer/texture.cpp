#include "raytracer/texture.h"

#include <stdexcept>

// OpenEXR
#include <ImfRgbaFile.h>
#include <ImfTestFile.h>

inline Raytracer::Color cubic(Raytracer::Color p[4], RT_FLOAT x)
{
	return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

inline Raytracer::Color bicubic(Raytracer::Color p[4][4], RT_FLOAT x, RT_FLOAT y)
{
	Raytracer::Color arr[4];
	arr[0] = cubic(p[0], y);
	arr[1] = cubic(p[1], y);
	arr[2] = cubic(p[2], y);
	arr[3] = cubic(p[3], y);
	return cubic(arr, x);
}

namespace Raytracer {

Texture::Texture()
{
    //ctor
}

Texture::Texture(const std::string &filename)
{
    open(filename);
}

Texture::Texture(const std::vector<Color, allocator<Color, 16>> &data, size_t width, size_t height)
{
    open(data, width, height);
}

Texture::~Texture()
{
    //dtor
}

void Texture::open(const std::string &filename)
{
    std::cout << "Opening '" << filename << "': " << std::flush;
    m_tex.clear();
    if(Imf::isOpenExrFile(filename.c_str()))
    {
        std::cout << "OpenEXR " << std::flush;
        Imf::RgbaInputFile file(filename.c_str());
        Imath::Box2i dw = file.dataWindow();
        m_width = dw.max.x - dw.min.x + 1;
        m_height = dw.max.y - dw.min.y + 1;
        std::cout << m_width << 'x' << m_height << std::flush;
        Imf::Rgba *pixels = new Imf::Rgba[m_width*m_height];
        file.setFrameBuffer(pixels - dw.min.x - dw.min.y*m_width, 1, m_width);
        file.readPixels(dw.min.y, dw.max.y);

        for(int y = 0; y < m_height; y++)
            for(int x = 0; x < m_width; x++)
                m_tex.push_back(Color(pixels[x+y*m_width]));
    }
    else
        throw std::runtime_error("Texture::open: file format not recognised");
    std::endl(std::cout);
}

void Texture::open(const std::vector<Color, allocator<Color, 16>> &data, size_t width, size_t height)
{
    m_tex.clear();
    m_width = width;
    m_height = height;
    for(int y = 0; y < m_height; y++)
            for(int x = 0; x < m_width; x++)
            {
                try
                {
                    m_tex.push_back(data[x+y*m_width]);
                }
                catch(...)
                {
                }
            }
}

Color Texture::get(RT_FLOAT u, RT_FLOAT v)
{
    while(u < 0)
        u += 1;
    while(v < 0)
        v += 1;
    while(u > 1)
        u -= 1;
    while(v > 1)
        v -= 1;
    u *= m_width;
    v *= m_height;
    RT_FLOAT U = std::floor(u);
    RT_FLOAT V = std::floor(v);

    Color p[4][4];
    for(int Y = 0; Y < 4; Y++)
        for(int X = 0; X < 4; X++)
        {
            int data_x = U-1+X, data_y = V-1+Y;
            if(data_x < 0)
                data_x = 0;
            if(data_y < 0)
                data_y = 0;
            if(data_x >= m_width)
                data_x = m_width-1;
            if(data_y >= m_height)
                data_y = m_height-1;
            p[X][Y] = m_tex.at(data_x+data_y*m_width);
        }
	return bicubic(p, u-U, v-V);
}

} // namespace Raytracer
