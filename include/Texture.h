#pragma once
#include <glad/glad.h>
#include <string>

class Texture
{
public:
    Texture(const std::string& path, bool flip = true);
    ~Texture() { glDeleteTextures(1, &m_id); }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void bind(unsigned int unit = 0) const;

private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
};
