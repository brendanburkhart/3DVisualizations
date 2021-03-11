#include "stdafx.h"
#include "Visualization.h"

#include "ShapeMeshes.h"

Visualization::Visualization() {
    meshes.push_back(ShapeMeshes::Cube());

    renderCamera = Camera();
    renderCamera.Position = Vector3(0, 0, 25);
    renderCamera.Target = Vector3::Origin();
}

void Visualization::Update(LONGLONG elapsed_time) {
    for (auto& mesh : meshes) {
        mesh.Rotation = Vector3(mesh.Rotation.X + 0.05f, mesh.Rotation.Y + 0.05f, mesh.Rotation.Z);
    }
}

void Visualization::Render(Device& renderDevice) {
    renderDevice.Clear(Color4(0.0, 0, 0, 1.0));
    renderDevice.Render(renderCamera, meshes);
}
