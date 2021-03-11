#pragma once

#include "stdafx.h"
#include "Mesh.h"

Wireframe::Wireframe() : color(Color4(1.0, 1.0, 1.0, 1.0)) {
    Vertices = std::vector<Vector3>();
    Edges = std::vector<std::pair<size_t, size_t>>();

    Rotation = Vector3(0, 0, 0);
    Position = Vector3(0, 0, 0);
}

Mesh::Face::Face() : A(NULL), B(NULL), C(NULL) {};

Mesh::Face::Face(int a, int b, int c) : A(a), B(b), C(c) { };

Mesh::Mesh() : color(Color4(1.0, 1.0, 1.0, 1.0)) {
    Vertices = std::vector<Vector3>();
    Faces = std::vector<Mesh::Face>();

    Rotation = Vector3(0, 0, 0);
    Position = Vector3(0, 0, 0);
}
