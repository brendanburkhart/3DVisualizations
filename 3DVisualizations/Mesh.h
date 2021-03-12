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

    std::vector<Vector3> Vertices;
    std::vector<Mesh::Face> Faces;
    std::vector<std::pair<size_t, size_t>> Edges;

    Color4 color;

    Mesh();
};
