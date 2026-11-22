#pragma once

#include <cstdint>

class Color
{
public:
    Color(std::uint8_t r = 0,
          std::uint8_t g = 0,
          std::uint8_t b = 0)
        : r_(r), g_(g), b_(b)
    {
    }

    std::uint8_t r() const { return r_; }
    std::uint8_t g() const { return g_; }
    std::uint8_t b() const { return b_; }

private:
    std::uint8_t r_;
    std::uint8_t g_;
    std::uint8_t b_;
};