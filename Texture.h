#pragma once
#include <string>

class Texture
{
public:
    Texture(const std::string& path, bool flip = true);
    void bind(unsigned int unit = 0) const;

private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
};
