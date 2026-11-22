#pragma once

#include <string>
#include <vector>

#include "Color.hpp"

class Image
{
public:
    Image(std::size_t width, std::size_t height);

    void setPixel(std::size_t x,
                  std::size_t y,
                  const Color& color);

    void save(const std::string& filename) const;

private:
    std::size_t width_;
    std::size_t height_;
    std::vector<Color> pixels_;
};