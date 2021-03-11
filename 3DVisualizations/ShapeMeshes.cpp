#include "stdafx.h"
#include "ShapeMeshes.h"

Mesh ShapeMeshes::Cube() {
    Mesh cube = Mesh(L"Cube", 8, 12);

    cube.color = Color4(0.25, 0.0, 0.5, 1.0);

    cube.Vertices[0] = Vector3(-3, 3, 3);
    cube.Vertices[1] = Vector3(3, 3, 3);
    cube.Vertices[2] = Vector3(-3, -3, 3);
    cube.Vertices[3] = Vector3(3, -3, 3);
    cube.Vertices[4] = Vector3(-3, 3, -3);
    cube.Vertices[5] = Vector3(3, 3, -3);
    cube.Vertices[6] = Vector3(-3, -3, -3);
    cube.Vertices[7] = Vector3(3, -3, -3);

    cube.Faces[0] = Mesh::Face(0, 1, 2);
    cube.Faces[1] = Mesh::Face(1, 2, 3);
    cube.Faces[2] = Mesh::Face(1, 3, 7);
    cube.Faces[3] = Mesh::Face(1, 5, 7);
    cube.Faces[4] = Mesh::Face(0, 1, 4);
    cube.Faces[5] = Mesh::Face(1, 4, 5);
    cube.Faces[6] = Mesh::Face(2, 3, 7);
    cube.Faces[7] = Mesh::Face(2, 6, 7);
    cube.Faces[8] = Mesh::Face(2, 4, 6);
    cube.Faces[9] = Mesh::Face(0, 4, 2);
    cube.Faces[10] = Mesh::Face(4, 5, 7);
    cube.Faces[11] = Mesh::Face(4, 6, 7);

    return cube;
}

// Vertices in anti-clockwise order
void AddPentagon(size_t index, int a, int b, int c, int d, int e, Mesh& mesh) {
    Vector3 ab = Vector3::Subtract(mesh.Vertices[b], mesh.Vertices[a]);
    Vector3 ad = Vector3::Subtract(mesh.Vertices[d], mesh.Vertices[a]);
    Vector3 normal = Vector3::Normalize(Vector3::Cross(ab, ad));

    Vector3 position_sum = Vector3::Add(mesh.Vertices[a], Vector3::Add(mesh.Vertices[c], mesh.Vertices[d]));
    Vector3 position = Vector3::Multiply(position_sum, 1 / 3.0);

    mesh.Faces[3*index] = Mesh::Face(a, b, c);
    mesh.Faces[3 * index].normal = normal;
    mesh.Faces[3 * index].position = position;

    mesh.Faces[3*index + 1] = Mesh::Face(a, c, d);
    mesh.Faces[3 * index + 1].normal = normal;
    mesh.Faces[3 * index + 1].position = position;

    mesh.Faces[3*index + 2] = Mesh::Face(a, d, e);
    mesh.Faces[3 * index + 2].normal = normal;
    mesh.Faces[3 * index + 2].position = position;
}

Mesh ShapeMeshes::Dodecahedron() {
    Mesh dodecahedron = Mesh(L"Dodecahedron", 20, 36);

    dodecahedron.color = Color4(0.25, 0.0, 0.5, 1.0);

    double scale = 2.5;

    int vertex = 0;
    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            for (int k = -1; k < 2; k += 2)
            {
                dodecahedron.Vertices[vertex++] = Vector3(scale * i, scale * j, scale * k);
            }
        }
    }

    double phi = (1.0 + sqrt(5.0)) / 2.0;

    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            dodecahedron.Vertices[vertex++] = Vector3(0.0, scale * i * phi, scale * j / phi);
        }
    }

    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            dodecahedron.Vertices[vertex++] = Vector3(scale * i / phi, 0.0, scale * j * phi);
        }
    }

    for (int i = -1; i < 2; i += 2)
    {
        for (int j = -1; j < 2; j += 2)
        {
            dodecahedron.Vertices[vertex++] = Vector3(scale * i * phi, scale * j / phi, 0.0);
        }
    }

    AddPentagon(0, 0, 12, 14, 4, 8, dodecahedron);
    AddPentagon(1, 6, 14, 12, 2, 10, dodecahedron);
    AddPentagon(2, 12, 0, 16, 17, 2, dodecahedron);
    AddPentagon(3, 14, 6, 19, 18, 4, dodecahedron);
    AddPentagon(4, 0, 8, 9, 1, 16, dodecahedron);
    AddPentagon(5, 18, 5, 9, 8, 4, dodecahedron);
    AddPentagon(6, 17, 3, 11, 10, 2, dodecahedron);
    AddPentagon(7, 19, 6, 10, 11, 7, dodecahedron);
    AddPentagon(8, 13, 3, 17, 16, 1, dodecahedron);
    AddPentagon(9, 15, 5, 18, 19, 7, dodecahedron);
    AddPentagon(10, 9, 5, 14, 13, 1, dodecahedron);
    AddPentagon(11, 11, 3, 13, 15, 7, dodecahedron);

    return dodecahedron;
}
