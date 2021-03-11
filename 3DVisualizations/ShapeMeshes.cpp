#include "stdafx.h"
#include "ShapeMeshes.h"

Mesh ShapeMeshes::Cube() {
    Mesh cube = Mesh(L"Cube", 8, 12);

    cube.Vertices[0] = Vector3(-3, 3, 3);
    cube.Vertices[1] = Vector3(3, 3, 3);
    cube.Vertices[2] = Vector3(-3, -3, 3);
    cube.Vertices[3] = Vector3(3, -3, 3);
    cube.Vertices[4] = Vector3(-3, 3, -3);
    cube.Vertices[5] = Vector3(3, 3, -3);
    cube.Vertices[6] = Vector3(-3, -3, -3);
    cube.Vertices[7] = Vector3(3, -3, -3);

    cube.Faces[0] = Mesh::Face(0, 1, 2);
    cube.Faces[1] = Mesh::Face(1, 2, 3);
    cube.Faces[2] = Mesh::Face(1, 3, 7);
    cube.Faces[3] = Mesh::Face(1, 5, 7);
    cube.Faces[4] = Mesh::Face(0, 1, 4);
    cube.Faces[5] = Mesh::Face(1, 4, 5);
    cube.Faces[6] = Mesh::Face(2, 3, 7);
    cube.Faces[7] = Mesh::Face(2, 6, 7);
    cube.Faces[8] = Mesh::Face(2, 4, 6);
    cube.Faces[9] = Mesh::Face(0, 4, 2);
    cube.Faces[10] = Mesh::Face(4, 5, 7);
    cube.Faces[11] = Mesh::Face(4, 6, 7);

    return cube;
}
