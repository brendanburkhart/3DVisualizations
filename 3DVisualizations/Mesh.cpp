#pragma once

#include "stdafx.h"
#include "Mesh.h"

Mesh::Face::Face() : A(NULL), B(NULL), C(NULL) { }

Mesh::Face::Face(int a, int b, int c) : A(a), B(b), C(c) { }

Mesh::Mesh()
    : Vertices(std::vector<Vector3>()),
      Faces(std::vector<Mesh::Face>()),
      Edges(std::vector<std::pair<size_t, size_t>>()) {}
