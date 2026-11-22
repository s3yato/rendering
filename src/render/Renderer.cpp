#include "Renderer.hpp"

#include <algorithm>
#include <cmath>
#include <chrono>
#include <iostream>
#include <array>
#include <vector>

#include "Cube.hpp"

Renderer::Renderer(int width, int height)
    : window_(nullptr),
      renderer_(nullptr),
      depthBuffer_(width, height),
      width_(width),
      height_(height)
{
    SDL_Init(SDL_INIT_VIDEO);

    window_ = SDL_CreateWindow(
        "My 3D Renderer",
        width_,
        height_,
        0
    );

    renderer_ = SDL_CreateRenderer(
        window_,
        nullptr
    );

    projection_ = Matrix4::perspective(
        90.0f,
        static_cast<float>(width_) / height_,
        0.1f,
        100.0f
    );

    camera_.setPosition(Vec3(0, 0, 5));

    transform_.setPosition(Vec3(0, 0, 0));
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

static Vec4 projectVertex(
    const Matrix4& model,
    const Matrix4& view,
    const Matrix4& projection,
    const Vec4& vertex
) {
    Vec4 world = model * vertex;
    Vec4 camera = view * world;
    Vec4 clip = projection * camera;

    return clip;
}

void perspectiveDivide(Vec4& vertex)
{
    if (vertex.w() == 0.0f) {
        return;
    }

    vertex = Vec4(
        vertex.x() / vertex.w(),
        vertex.y() / vertex.w(),
        vertex.z() / vertex.w(),
        vertex.w()
    );
}

static int screenX(float x, int width) {
    return static_cast<int>(
        (x + 1.0f) * 0.5f * width
    );
}

static int screenY(float y, int height) {
    return static_cast<int>(
        (1.0f - y) * 0.5f * height
    );
}

void Renderer::drawLine(
    int x1,
    int y1,
    int x2,
    int y2
) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = dx - dy;

    while (true) {
        drawPixel(x1, y1, 1.0f);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void Renderer::drawPixel(int x, int y, float brightness)
{
    brightness = std::clamp(brightness, 0.0f, 1.0f);

    int value = static_cast<int>(brightness * 255.0f);

    SDL_SetRenderDrawColor(
        renderer_,
        value,
        value,
        value,
        255
    );

    SDL_RenderPoint(
        renderer_,
        x,
        y
    );
}

Vec3 calculateNormal(
    const Vec3& a,
    const Vec3& b,
    const Vec3& c
) {
    Vec3 ab = b - a;
    Vec3 ac = c - a;

    return ab.cross(ac).normalized();
}

static Vec3 transformNormal(const Matrix4& model, const Vec3& normal)
{
    Vec4 n(
        normal.x(),
        normal.y(),
        normal.z(),
        0.0f
    );

    Vec4 transformed = model * n;

    return Vec3(
        transformed.x(),
        transformed.y(),
        transformed.z()
    ).normalized();
}

bool insideNearPlane(const Vec4& v)
{
    return v.z() <= -0.1f;
}

Vec4 intersectNearPlane(
    const Vec4& a,
    const Vec4& b
) {
    constexpr float nearPlane = -0.1f;

    float t =
        (nearPlane - a.z()) /
        (b.z() - a.z());

    return Vec4(
        a.x() + (b.x() - a.x()) * t,
        a.y() + (b.y() - a.y()) * t,
        nearPlane,
        1.0f
    );
}

std::vector<std::array<Vec4, 3>> clipTriangleNearPlane(
    const Vec4& a,
    const Vec4& b,
    const Vec4& c
)
{
    std::vector<Vec4> input = {
        a, b, c
    };

    std::vector<Vec4> output;

    for (int i = 0; i < 3; ++i) {

        const Vec4& current = input[i];
        const Vec4& next = input[(i + 1) % 3];

        bool currentInside =
            insideNearPlane(current);

        bool nextInside =
            insideNearPlane(next);

        if (currentInside && nextInside) {

            output.push_back(next);

        }
        else if (currentInside && !nextInside) {

            output.push_back(
                intersectNearPlane(
                    current,
                    next
                )
            );

        }
        else if (!currentInside && nextInside) {

            output.push_back(
                intersectNearPlane(
                    current,
                    next
                )
            );

            output.push_back(next);
        }
    }

    std::vector<std::array<Vec4, 3>> triangles;

    if (output.size() == 3) {

        triangles.push_back({
            output[0],
            output[1],
            output[2]
        });

    }
    else if (output.size() == 4) {

        triangles.push_back({
            output[0],
            output[1],
            output[2]
        });

        triangles.push_back({
            output[0],
            output[2],
            output[3]
        });
    }

    return triangles;
}

std::vector<Vec4> clipPolygonAgainstPlane(
    const std::vector<Vec4>& polygon,
    float planeZ
)
{
    std::vector<Vec4> result;

    if (polygon.empty()) {
        return result;
    }

    for (std::size_t i = 0; i < polygon.size(); ++i) {

        const Vec4& current =
            polygon[i];

        const Vec4& previous =
            polygon[(i + polygon.size() - 1) % polygon.size()];

        bool currentInside =
            current.z() >= planeZ;

        bool previousInside =
            previous.z() >= planeZ;

        if (currentInside != previousInside) {

            float t =
                (planeZ - previous.z()) /
                (current.z() - previous.z());

            Vec4 intersection =
                previous +
                (current - previous) * t;

            result.push_back(intersection);
        }

        if (currentInside) {
            result.push_back(current);
        }
    }

    return result;
}

void Renderer::run() {
    bool running = true;

    SDL_Event event;

    auto previousT = std::chrono::steady_clock::now();

    SDL_SetWindowRelativeMouseMode(window_, true);
    Mesh cube = Cube::create(2.0f);

    while (running) {
        auto currentT = std::chrono::steady_clock::now();
        float deltaTime =
            std::chrono::duration<float>(currentT - previousT).count();
        previousT = currentT;

        float mouseX = 0.0f;
        float mouseY = 0.0f;
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                mouseX = event.motion.xrel;
                mouseY = event.motion.yrel;
            }
        }
        
        float sensitivity = 0.1f;
        
        Vec3 cameraRotation = camera_.rotation();
        
        camera_.setRotation(
            Vec3(
                cameraRotation.x() - mouseY * sensitivity,
                cameraRotation.y() + mouseX * sensitivity,
                cameraRotation.z()
            )
        );

        const bool* keyboard = SDL_GetKeyboardState(nullptr);

        float speed = 3.0f;

        Vec3 movement(0.0f, 0.0f, 0.0f);

        Vec3 look = camera_.forward();

        Vec3 forward(
            look.x(),
            0.0f,
            look.z()
        );

        forward = forward.normalized();

        Vec3 right(
            -forward.z(),
            0.0f,
            forward.x()
        );

        right = right.normalized();

        if (keyboard[SDL_SCANCODE_W]) {
            movement = movement + forward;
        }

        if (keyboard[SDL_SCANCODE_S]) {
            movement = movement - forward;
        }

        if (keyboard[SDL_SCANCODE_D]) {
            movement = movement + right;
        }

        if (keyboard[SDL_SCANCODE_A]) {
            movement = movement - right;
        }

        if (movement.length() > 0.0f) {
            movement = movement.normalized();
        }

        camera_.move(movement * (speed * deltaTime));
    
        SDL_SetRenderDrawColor(
            renderer_,
            20,
            20,
            20,
            255
        );

        SDL_RenderClear(renderer_);
        depthBuffer_.clear();

        Vec3 rotation = transform_.rotation();

        Vec3 new_rotation = Vec3(
            rotation.x(),
            rotation.y() + 45.0f * deltaTime,
            rotation.z()
        );

        transform_.setRotation(new_rotation);

        Matrix4 model = transform_.modelMatrix();
        Matrix4 view = camera_.viewMatrix();

        for (const Triangle& triangle : cube.triangles()) {
            Vec3 a = triangle.a();
            Vec3 b = triangle.b();
            Vec3 c = triangle.c();

            Vec4 a4(a.x(), a.y(), a.z(), 1.0f);
            Vec4 b4(b.x(), b.y(), b.z(), 1.0f);
            Vec4 c4(c.x(), c.y(), c.z(), 1.0f);

            a4 = model * a4;
            b4 = model * b4;
            c4 = model * c4;

            Vec3 worldA(
                a4.x(),
                a4.y(),
                a4.z()
            );

            Vec3 worldB(
                b4.x(),
                b4.y(),
                b4.z()
            );

            Vec3 worldC(
                c4.x(),
                c4.y(),
                c4.z()
            );

            Vec3 normal = calculateNormal(
                worldA,
                worldB,
                worldC
            );

            Vec3 facePosition =
                (worldA + worldB + worldC) / 3.0f;
            Vec3 cameraDirection =
                camera_.position() - facePosition;
            
            cameraDirection =
                cameraDirection.normalized();
            
            if (normal.dot(cameraDirection) <= 0.0f) {
                continue;
            }

            a4 = view * a4;
            b4 = view * b4;
            c4 = view * c4;

            auto clippedTriangles =
                clipTriangleNearPlane(
                    a4,
                    b4,
                    c4
                );

            for (const auto& clipped : clippedTriangles) {

                Vec4 clippedA = projection_ * clipped[0];
                Vec4 clippedB = projection_ * clipped[1];
                Vec4 clippedC = projection_ * clipped[2];

                perspectiveDivide(clippedA);
                perspectiveDivide(clippedB);
                perspectiveDivide(clippedC);

                Vec3 screenA(
                    screenX(clippedA.x(), width_),
                    screenY(clippedA.y(), height_),
                    clippedA.z()
                );

                Vec3 screenB(
                    screenX(clippedB.x(), width_),
                    screenY(clippedB.y(), height_),
                    clippedB.z()
                );

                Vec3 screenC(
                    screenX(clippedC.x(), width_),
                    screenY(clippedC.y(), height_),
                    clippedC.z()
                );

                rasterizeTriangle(
                    screenA,
                    screenB,
                    screenC,
                    normal,
                    facePosition
                );
            }
        }
        SDL_RenderPresent(renderer_);
    }
}
float edge(const Vec3& a, const Vec3& b, const Vec3& p) {
    return (b.x() - a.x()) * (p.y() - a.y()) - (b.y() - a.y()) * (p.x() - a.x());
}

float interpolateDepth(
    const Vec3& a,
    const Vec3& b,
    const Vec3& c,
    const Vec3& p
) {
    float area = edge(a, b, c);

    float wA = edge(b, c, p) / area;
    float wB = edge(c, a, p) / area;
    float wC = edge(a, b, p) / area;

    float depth = 
        wA * a.z() +
        wB * b.z() +
        wC * c.z();

    return depth;
}

float Renderer::calculateLighting(
    const Vec3& normal,
    const Vec3& facePosition
) const
{
    Vec3 lightDirection = camera_.position() - facePosition;
    lightDirection = lightDirection.normalized();

    float diffuse =
        normal.normalized().dot(lightDirection);

    diffuse = std::max(0.0f, diffuse);

    constexpr float ambient = 0.30f;

    return ambient + 0.70f * diffuse;
}

void Renderer::rasterizeTriangle(
    const Vec3& a,
    const Vec3& b,
    const Vec3& c,
    const Vec3& normal,
    const Vec3& facePosition
) {
    float x_min = std::min(
        std::min(a.x(), b.x()),
        c.x()
    );
    float x_max = std::max(
        std::max(a.x(), b.x()),
        c.x()
    );
    float y_min = std::min(
        std::min(a.y(), b.y()),
        c.y()
    );
    float y_max = std::max(
        std::max(a.y(), b.y()),
        c.y()
    );
    x_min = std::max(x_min, 0.0f);
    y_min = std::max(y_min, 0.0f);

    x_max = std::min(x_max, static_cast<float>(width_ - 1));
    y_max = std::min(y_max, static_cast<float>(height_ - 1));


    for (int y = static_cast<int>(y_min);
    y <= static_cast<int>(y_max);
    ++y) {

        for (int x = static_cast<int>(x_min);
                x <= static_cast<int>(x_max);
                ++x) {

            Vec3 p(
                static_cast<float>(x),
                static_cast<float>(y),
                0.0f
            );

            float e1 = edge(a, b, p);
            float e2 = edge(b, c, p);
            float e3 = edge(c, a, p);

            if (
                (e1 >= 0 && e2 >= 0 && e3 >= 0) ||
                (e1 <= 0 && e2 <= 0 && e3 <= 0)
            ) {
                float depth =
                    interpolateDepth(a, b, c, p);

                if (depthBuffer_.testAndSet(x, y, depth)) {

                    // float brightness =
                    //     calculateLighting(
                    //         normal,
                    //         facePosition
                    //     );

                    drawPixel(
                        x,
                        y,
                        1.0f
                    );
                }
            }
        }
    }
}

