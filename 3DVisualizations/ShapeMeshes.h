#pragma once

#include "Mesh.h"

class ShapeMeshes
{
public:
    static Mesh CubeMesh();
    static Mesh DodecahedronMesh();
    static Wireframe DodecahedronWireframe();
    static Wireframe EmbeddedCube();
};
