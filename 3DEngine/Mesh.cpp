#pragma once

#include "stdafx.h"
#include "Mesh.h"

Mesh::Face::Face () : A (NULL), B (NULL), C (NULL) {};

Mesh::Face::Face (int a, int b, int c) : A (a), B (b), C (c) {};

Mesh::Mesh (std::string name, int verticesCount, int facesCount) {
    Vertices = std::vector<Vector3> (verticesCount);
    Faces = std::vector<Mesh::Face> (facesCount);

    Name = name;

    Rotation = Vector3 (0, 0, 0);
    Position = Vector3 (0, 0, 0);
}