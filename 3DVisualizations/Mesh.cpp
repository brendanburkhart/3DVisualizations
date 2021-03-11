#pragma once

#include "stdafx.h"
#include "Mesh.h"

Mesh::Face::Face() : A(NULL), B(NULL), C(NULL) {};

Mesh::Face::Face(int a, int b, int c) : A(a), B(b), C(c) { };

Mesh::Mesh(std::wstring name, int verticesCount, int facesCount)
    : color(Color4(1.0, 1.0, 1.0, 1.0)) {
    Vertices = std::vector<Vector3>(verticesCount);
    Faces = std::vector<Mesh::Face>(facesCount);

    Name = name;

    Rotation = Vector3(0, 0, 0);
    Position = Vector3(0, 0, 0);
}

void Mesh::ComputeNormals() {
    for (auto& face: Faces)
    {
        Vector3 a = Vector3::Subtract(Vertices[face.B], Vertices[face.A]);
        Vector3 b = Vector3::Subtract(Vertices[face.C], Vertices[face.A]);
        Vector3 n = Vector3::Cross(a, b);
        n = Vector3::Normalize(n);

        double d = Vector3::Dot(a, n);
        if (d < 0.0) {
            face.normal = Vector3::Negate(n);
        }
        else {
            face.normal = n;
        }
    }
}
