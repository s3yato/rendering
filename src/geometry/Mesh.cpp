#include "Mesh.hpp"

void Mesh::addTriangle(const Triangle& triangle) {
    triangles_.push_back(triangle);
}

const std::vector<Triangle>& Mesh::triangles() const {
    return triangles_;
}

