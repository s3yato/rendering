#pragma once

#include <vector>
#include <algorithm>

class DepthBuffer {
public:
    DepthBuffer(int width, int height);

    void clear();

    float get(int x, int y) const;
    void set(int x, int y, float depth);
    bool testAndSet(int x, int y, float depth);

private:
    int width_;
    int height_;
    std::vector<float> depth_;
};