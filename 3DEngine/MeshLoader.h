#pragma once

#include <vector>
#include <string>

#include "Mesh.h"

class MeshLoader {
public:
    static void LoadMesh (std::vector<Mesh> &Meshes, std::wstring fileName);
};