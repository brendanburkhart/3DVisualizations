#pragma once

#include "Vector3.h"
#include <string>
#include <vector>

class Mesh {
public:
    std::string Name;
    std::vector<Vector3> Vertices;
    Vector3 Position;
    Vector3 Rotation;

    Mesh (std::string name, int verticesCount);
};