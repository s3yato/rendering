#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <algorithm>

#include "Matrix4.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "DepthBuffer.hpp"
#include "Light.hpp"
#include "Vec3.hpp"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void run();

    void rasterizeTriangle(
        const Vec3& a,
        const Vec3& b,
        const Vec3& c,
        const Vec3& normal,
        const Vec3& facePosition
    );
private:
    void drawPixel(int x, int y, float brightness);
    void drawLine(int x1, int y1, int x2, int y2);

    void drawTriangle(
        int x1, int y1,
        int x2, int y2,
        int x3, int y3
    );

    float calculateLighting(
        const Vec3& normal,
        const Vec3& facePosition
    ) const;

    SDL_Window* window_;
    SDL_Renderer* renderer_;

    int width_;
    int height_;

    DepthBuffer depthBuffer_;
    Camera camera_;
    Transform transform_;

    Matrix4 projection_;
};