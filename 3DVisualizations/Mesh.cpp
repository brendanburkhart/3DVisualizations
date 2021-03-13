#pragma once

#include "stdafx.h"
#include "Mesh.h"

Mesh::Face::Face() : A(0), B(0), C(0), color(Color4(0.0, 0.0, 0.0, 1.0)) { }

Mesh::Face::Face(int a, int b, int c) : A(a), B(b), C(c), color(Color4(0.0, 0.0, 0.0, 1.0)) { }

Mesh::Mesh()
    : Vertices(std::vector<Vector3>()),
      Faces(std::vector<Mesh::Face>()),
      Edges(std::vector<std::pair<size_t, size_t>>()) {}
