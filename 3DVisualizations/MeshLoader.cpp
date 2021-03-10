#include "stdafx.h"

#include <locale>
#include <codecvt>
#include <fstream>

#include  "json.h"

#include "MeshLoader.h"

void MeshLoader::LoadMesh (std::vector<Mesh> &Meshes, std::wstring fileName) {
    Json::Value root;

    std::ifstream meshData (fileName, std::ifstream::binary);

    if (meshData.fail()) {
        OutputDebugString (L"Ermm....\n");
    } else {
        meshData >> root;
    }

    meshData.close ();

    const Json::Value meshes = root ["meshes"];

    const Json::Value vertices = meshes [0].get ("vertices", NULL);

    const Json::Value indices = meshes [0].get ("indices", NULL);

    const Json::Int uvCount = meshes [0].get ("uvCount", NULL).asInt ();

    auto dataStep = 1;

    switch ((int)uvCount) {
    case 0:
        dataStep = 6;
        break;
    case 1:
        dataStep = 8;
        break;
    case 2:
        dataStep = 10;
        break;
    }

    auto verticeCount = vertices.size () / dataStep;
    auto faceCount = indices.size () / 3;

    std::string name = meshes [0].get ("name", NULL).asString ();
    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring meshName = converter.from_bytes (name);
    
    Mesh mesh = Mesh (meshName, verticeCount, faceCount);

    // Set all vertices
    for (auto index = 0; index < verticeCount; index++) {
        auto x = (float)vertices [index * dataStep].asFloat ();
        auto y = (float)vertices [index * dataStep + 1].asFloat ();
        auto z = (float)vertices [index * dataStep + 2].asFloat ();

        mesh.Vertices [index] = Vector3 (x, y, z);
    }

    // Set all faces
    for (auto index = 0; index < faceCount; index++) {
        auto a = (int)indices [index * 3].asInt ();
        auto b = (int)indices [index * 3 + 1].asInt ();
        auto c = (int)indices [index * 3 + 2].asInt ();
        mesh.Faces [index] = Mesh::Face { a, b, c };
    }

    // Set the position
    auto position = meshes [0].get ("position", NULL);
    mesh.Position = Vector3 ((double)position [0].asDouble (), (double)position [1].asDouble (), (double)position [2].asDouble ());

    // Set the rotation
    auto rotation = meshes [0].get ("rotation", NULL);
    mesh.Rotation = Vector3 ((double)rotation [0].asDouble (), (double)rotation [1].asDouble (), (double)rotation [2].asDouble ());

    // Add mesh to vector of meshes for rendering
    Meshes.push_back (mesh);
}