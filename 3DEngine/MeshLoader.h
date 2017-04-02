#pragma once

#include "json/json.h"
#include <vector>
#include <string>
#include <fstream>

#include "Mesh.h"

class MeshLoader {
public:
    static void LoadMesh (std::vector<Mesh> &Meshes, std::wstring fileName);
};