#include "Image.hpp"

#include <fstream>
#include <iostream>


Image::Image(std::size_t width, std::size_t height)
    : width_(width),
        height_(height),
        pixels_(width * height)
{
}


void Image::save(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл!\n";
        return;
    }

    file << "P3\n";
    file << width << " " << height << "\n";
    file << "255\n";

    for (std::size_t i = 0; i < pixels_.size(); i++) {
        std::uint8_t r = pixels_[i].getR();
        std::uint8_t g = pixels_[i].getG();
        std::uint8_t b = pixels_[i].getB();
        file << static_cast<int>(r)
            << " "
            << static_cast<int>(g)
            << " "
            << static_cast<int>(b)
            << "\n";
    }
}

void Image::setPixel(std::size_t x, std::size_t y,const Color& color) {
    if (x >= width || y >= height)
        return;

    std::size_t index = y * width + x;
    pixels_[index] = color;
}
