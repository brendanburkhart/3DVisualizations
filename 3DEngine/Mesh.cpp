#pragma once

#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh (std::string name, int verticesCount) {
    Vertices = std::vector<Vector3> (verticesCount);
    Name = name;
}