cmake_minimum_required(VERSION 3.20)

project(render LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)


# ========================
# Geometry
# ========================

add_library(geometry STATIC
    src/geometry/Cube.cpp
    src/geometry/Mesh.cpp
    src/geometry/Triangle.cpp
)

target_include_directories(geometry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
)

# ========================
# Math
# ========================

add_library(math STATIC
    src/math/Matrix4.cpp
)

target_include_directories(math PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
)


# ========================
# Scene
# ========================

add_library(scene STATIC
    src/scene/Transform.cpp
    src/scene/Light.cpp
)

target_include_directories(scene PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scene
)

target_link_libraries(scene PUBLIC
    math
    geometry
)


# ========================
# Render
# ========================

add_library(render_engine STATIC
    src/render/Camera.cpp
    src/render/Renderer.cpp
    src/render/DepthBuffer.cpp
)

target_include_directories(render_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scene
    ${CMAKE_CURRENT_SOURCE_DIR}/src/render
)

target_link_libraries(render_engine PUBLIC
    math
    scene
    geometry
    SDL3::SDL3
)


# ========================
# Application
# ========================

add_executable(render
    src/main.cpp
)

target_link_libraries(render PRIVATE
    render_engine
)