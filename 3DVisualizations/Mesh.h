#pragma once

#include "Vector3.h"
#include <string>
#include <vector>

class Mesh {
public:
    struct Face {
        int A, B, C;

        Face();
        Face(int a, int b, int c);
    };

    std::wstring Name;
    std::vector<Vector3> Vertices;
    std::vector<Mesh::Face> Faces;
    Vector3 Position;
    Vector3 Rotation;

    Mesh(std::wstring name, int verticesCount, int facesCount);
};