#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H

#include "raytracer/allocator.h"
#include "raytracer/color.h"

namespace Raytracer {

class Texture
{
    public:
        Texture();
        Texture(const std::string &filename);
        Texture(const std::vector<Color, allocator<Color, 16>> &data, size_t width, size_t height);
        virtual ~Texture();
        void open(const std::string &filename);
        void open(const std::vector<Color, allocator<Color, 16>> &data, size_t width, size_t height);
        Color get(RT_FLOAT u, RT_FLOAT v);
    protected:
        std::size_t m_width, m_height;
        std::vector <Color, allocator<Color, 16>> m_tex;
    private:
};

} // namespace Raytracer

#endif // RAYTRACER_TEXTURE_H
