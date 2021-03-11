#pragma once

#include <string>
#include <vector>

#include "Color4.h"
#include "Vector3.h"

class Mesh {
public:
    struct Face {
        int A, B, C;
        Vector3 normal;
        Vector3 position;

        Face();
        Face(int a, int b, int c);
    };

    std::wstring Name;
    std::vector<Vector3> Vertices;
    std::vector<Mesh::Face> Faces;

    Color4 color;

    Vector3 Position;
    Vector3 Rotation;

    Mesh(std::wstring name, int verticesCount, int facesCount);

    void ComputeNormals();
};
