#pragma once

#include <vector>

#include "Triangle.hpp"

class Mesh {
    public:
        Mesh() = default;

        void addTriangle(const Triangle& triangle);

        const std::vector<Triangle>& triangles() const;
    private:
        std::vector<Triangle> triangles_;
};