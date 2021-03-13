#include "stdafx.h"
#include "ShapeMeshes.h"

Mesh ShapeMeshes::Cube() {
    Mesh cube;

    cube.Vertices.push_back(Vector3(-3, 3, 3));
    cube.Vertices.push_back(Vector3(3, 3, 3));
    cube.Vertices.push_back(Vector3(-3, -3, 3));
    cube.Vertices.push_back(Vector3(3, -3, 3));
    cube.Vertices.push_back(Vector3(-3, 3, -3));
    cube.Vertices.push_back(Vector3(3, 3, -3));
    cube.Vertices.push_back(Vector3(-3, -3, -3));
    cube.Vertices.push_back(Vector3(3, -3, -3));

    cube.Faces.push_back(Mesh::Face(0, 1, 2));
    cube.Faces.push_back(Mesh::Face(1, 2, 3));
    cube.Faces.push_back(Mesh::Face(1, 3, 7));
    cube.Faces.push_back(Mesh::Face(1, 5, 7));
    cube.Faces.push_back(Mesh::Face(0, 1, 4));
    cube.Faces.push_back(Mesh::Face(1, 4, 5));
    cube.Faces.push_back(Mesh::Face(2, 3, 7));
    cube.Faces.push_back(Mesh::Face(2, 6, 7));
    cube.Faces.push_back(Mesh::Face(2, 4, 6));
    cube.Faces.push_back(Mesh::Face(0, 4, 2));
    cube.Faces.push_back(Mesh::Face(4, 5, 7));
    cube.Faces.push_back(Mesh::Face(4, 6, 7));

    return cube;
}

// Vertices in anti-clockwise order
void addPentagonFace(size_t index, int a, int b, int c, int d, int e, const Color4& color, Mesh& mesh) {
    Vector3 ab = Vector3::Subtract(mesh.Vertices[b], mesh.Vertices[a]);
    Vector3 ad = Vector3::Subtract(mesh.Vertices[d], mesh.Vertices[a]);
    Vector3 normal = Vector3::Normalize(Vector3::Cross(ab, ad));

    Vector3 position_sum = Vector3::Add(mesh.Vertices[a], Vector3::Add(mesh.Vertices[c], mesh.Vertices[d]));
    Vector3 position = Vector3::Multiply(position_sum, 1 / 3.0);

    auto first = Mesh::Face(a, b, c);
    first.normal = normal;
    first.position = position;

    auto second = Mesh::Face(a, c, d);
    second.normal = normal;
    second.position = position;

    auto third = Mesh::Face(a, d, e);
    third.normal = normal;
    third.position = position;

    first.color = color;
    second.color = color;
    third.color = color;

    mesh.Faces.push_back(first);
    mesh.Faces.push_back(second);
    mesh.Faces.push_back(third);
}

// Vertices in anti-clockwise order
void addCubeFace(size_t index, int a, int b, int c, int d, Mesh& mesh) {
    Vector3 ab = Vector3::Subtract(mesh.Vertices[b], mesh.Vertices[a]);
    Vector3 ac = Vector3::Subtract(mesh.Vertices[c], mesh.Vertices[a]);
    Vector3 normal = Vector3::Normalize(Vector3::Cross(ab, ac));

    Vector3 position_sum = Vector3::Add(mesh.Vertices[a], mesh.Vertices[b]);
    position_sum = Vector3::Add(position_sum, mesh.Vertices[c]);
    position_sum = Vector3::Add(position_sum, mesh.Vertices[d]);
    Vector3 position = Vector3::Multiply(position_sum, 1 / 4.0);

    auto first = Mesh::Face(a, b, c);
    first.normal = normal;
    first.position = position;

    auto second = Mesh::Face(a, c, d);
    second.normal = normal;
    second.position = position;
    
    mesh.Faces.push_back(first);
    mesh.Faces.push_back(second);
}

void addDodecahedronVertices(std::vector<Vector3>& vertices) {
    double scale = 2.5;

    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            for (int k = -1; k < 2; k += 2)
            {
                vertices.push_back(Vector3(scale * i, scale * j, scale * k));
            }
        }
    }

    double phi = (1.0 + sqrt(5.0)) / 2.0;

    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            vertices.push_back(Vector3(0.0, scale * i * phi, scale * j / phi));
        }
    }

    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            vertices.push_back(Vector3(scale * i / phi, 0.0, scale * j * phi));
        }
    }

    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            vertices.push_back(Vector3(scale * i * phi, scale * j / phi, 0.0));
        }
    }
}

void addEdge(Mesh& mesh, size_t a, size_t b) {
    mesh.Edges.push_back(std::pair<size_t, size_t>(a, b));
}

Mesh ShapeMeshes::Dodecahedron() {
    Mesh dodecahedron;

    addDodecahedronVertices(dodecahedron.Vertices);

    addPentagonFace(0, 0, 12, 14, 4, 8, Color4(0.0, 0.75, 0.25, 1.0), dodecahedron);
    addPentagonFace(1, 6, 14, 12, 2, 10, Color4(0.75, 0.0, 0.25, 1.0), dodecahedron);
    addPentagonFace(2, 12, 0, 16, 17, 2, Color4(0.75, 0.0, 0.25, 1.0), dodecahedron);
    addPentagonFace(3, 14, 6, 19, 18, 4, Color4(0.5, 0.75, 0.0, 1.0), dodecahedron);
    addPentagonFace(4, 0, 8, 9, 1, 16, Color4(0.0, 0.75, 0.25, 1.0), dodecahedron);
    addPentagonFace(5, 18, 5, 9, 8, 4, Color4(0.0, 0.25, 0.75, 1.0), dodecahedron);
    addPentagonFace(6, 17, 3, 11, 10, 2, Color4(0.5, 0.75, 0.0, 1.0), dodecahedron);
    addPentagonFace(7, 19, 6, 10, 11, 7, Color4(0.25, 0.0, 0.75, 1.0), dodecahedron);
    addPentagonFace(8, 13, 3, 17, 16, 1, Color4(0.75, 0.25, 0.0, 1.0), dodecahedron);
    addPentagonFace(9, 15, 5, 18, 19, 7, Color4(0.75, 0.25, 0.0, 1.0), dodecahedron);
    addPentagonFace(10, 9, 5, 15, 13, 1, Color4(0.25, 0.0, 0.75, 1.0), dodecahedron);
    addPentagonFace(11, 11, 3, 13, 15, 7, Color4(0.0, 0.25, 0.75, 1.0), dodecahedron);

    addEdge(dodecahedron, 0, 8);
    addEdge(dodecahedron, 0, 12);
    addEdge(dodecahedron, 0, 16);

    addEdge(dodecahedron, 1, 9);
    addEdge(dodecahedron, 1, 13);
    addEdge(dodecahedron, 1, 16);

    addEdge(dodecahedron, 2, 10);
    addEdge(dodecahedron, 2, 12);
    addEdge(dodecahedron, 2, 17);

    addEdge(dodecahedron, 3, 11);
    addEdge(dodecahedron, 3, 13);
    addEdge(dodecahedron, 3, 17);

    addEdge(dodecahedron, 4, 8);
    addEdge(dodecahedron, 4, 14);
    addEdge(dodecahedron, 4, 18);

    addEdge(dodecahedron, 5, 9);
    addEdge(dodecahedron, 5, 15);
    addEdge(dodecahedron, 5, 18);

    addEdge(dodecahedron, 6, 10);
    addEdge(dodecahedron, 6, 14);
    addEdge(dodecahedron, 6, 19);

    addEdge(dodecahedron, 7, 11);
    addEdge(dodecahedron, 7, 15);
    addEdge(dodecahedron, 7, 19);

    addEdge(dodecahedron, 8, 9);
    addEdge(dodecahedron, 10, 11);
    addEdge(dodecahedron, 12, 14);
    addEdge(dodecahedron, 13, 15);
    addEdge(dodecahedron, 16, 17);
    addEdge(dodecahedron, 18, 19);

    return dodecahedron;
}

Mesh ShapeMeshes::EmbeddedCube() {
    Mesh cube;

    addDodecahedronVertices(cube.Vertices);

    addCubeFace(0, 0, 14, 18, 9, cube);
    addCubeFace(1, 9, 18, 7, 13, cube);
    addCubeFace(3, 13, 7, 10, 17, cube);
    addCubeFace(4, 17, 10, 14, 0, cube);
    addCubeFace(5, 0, 9, 13, 17, cube);
    addCubeFace(6, 10, 7, 18, 14, cube);

    addEdge(cube, 0, 9);
    addEdge(cube, 0, 14);
    addEdge(cube, 0, 17);

    addEdge(cube, 7, 10);
    addEdge(cube, 7, 13);
    addEdge(cube, 7, 18);

    addEdge(cube, 9, 13);
    addEdge(cube, 9, 18);

    addEdge(cube, 10, 14);
    addEdge(cube, 10, 17);
    addEdge(cube, 13, 17);
    addEdge(cube, 14, 18);

    return cube;
}
