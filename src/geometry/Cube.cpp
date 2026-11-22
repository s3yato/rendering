#include "Cube.hpp"

Mesh Cube::create(float size) {
    float h = size * 0.5f;
    Vec3 v0(-h,-h,-h);
    Vec3 v1( h,-h,-h);
    Vec3 v2( h, h,-h);
    Vec3 v3(-h, h,-h);
    
    Vec3 v4(-h,-h, h);
    Vec3 v5( h,-h, h);
    Vec3 v6( h, h, h);
    Vec3 v7(-h, h, h);

    Mesh mesh;

    // передняя грань
    mesh.addTriangle(
        Triangle(v4, v5, v6));

    mesh.addTriangle(
        Triangle(v4, v6, v7)
    );

    // задняя грань 
    mesh.addTriangle(
        Triangle(v0, v2, v1)
    );
    
    mesh.addTriangle(
        Triangle(v0, v3, v2)
    );

    // вверхняя грань
    mesh.addTriangle(
        Triangle(v7, v6, v2)
    );
    mesh.addTriangle(
        Triangle(v7, v2, v3)
    );

    // нижняя грань
    mesh.addTriangle(
        Triangle(v4, v1, v5)
    );

    mesh.addTriangle(
        Triangle(v4, v0, v1)
    );

    // левая грань
    mesh.addTriangle(
        Triangle(v0, v4, v7)
    );
    mesh.addTriangle(
        Triangle(v0, v7, v3)
    );

    // правая грань
    mesh.addTriangle(
        Triangle(v5, v1, v2)
    );
    mesh.addTriangle(
        Triangle(v5, v2, v6)
    );
    return mesh;
}