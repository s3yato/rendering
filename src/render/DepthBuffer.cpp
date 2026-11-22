#include "DepthBuffer.hpp"

DepthBuffer::DepthBuffer(int width, int height)
    : width_(width),
    height_(height),
    depth_(width * height)
{
}

void DepthBuffer::clear() {
    std::fill(depth_.begin(), depth_.end(), 1.0f);
}

float DepthBuffer::get(int x, int y) const {
    int index = y * width_ + x;

    return depth_[index];
}

void DepthBuffer::set(int x, int y, float depth) {
    int index = y * width_ + x;

    depth_[index] = depth;
}

bool DepthBuffer::testAndSet(int x, int y, float depth) {
    int index = y * width_ + x;

    float old_depth = depth_[index];

    if (old_depth < depth) {
        return false;
    }

    set(x, y, depth);
    return true;
}