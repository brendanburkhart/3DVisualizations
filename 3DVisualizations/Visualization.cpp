#include "stdafx.h"
#include "Visualization.h"

#include "ShapeMeshes.h"

Visualization::Visualization() {
    auto mesh = ShapeMeshes::Dodecahedron();
    meshes.push_back(mesh);

    renderCamera = Camera();
    renderCamera.Position = Vector3(25, 0, 0);
    renderCamera.Target = Vector3::Origin();
    renderCamera.Light = Vector3(25, 25, 25);
}

void Visualization::Update(double elapsed_seconds) {
    for (auto& mesh : meshes) {
        double rotation = 0.2 * (elapsed_seconds);
        mesh.Rotation = Vector3(mesh.Rotation.X + rotation, mesh.Rotation.Y + rotation, mesh.Rotation.Z);
    }
}

void Visualization::Render(Device& renderDevice) {
    renderDevice.Clear(Color4(1.0, 1.0, 1.0, 1.0));
    renderDevice.Render(renderCamera, meshes);
    renderDevice.Wireframe(renderCamera, meshes);
}
