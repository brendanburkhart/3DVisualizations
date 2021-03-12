#include "stdafx.h"
#include "Visualization.h"

#include "ShapeMeshes.h"

Visualization::Visualization() {
    dodecahedron = ShapeMeshes::Dodecahedron();
    cube = ShapeMeshes::EmbeddedCube();

    renderCamera = Camera();
    renderCamera.Position = Vector3(25, 0, 0);
    renderCamera.Target = Vector3::Origin();
    renderCamera.Light = Vector3(25, 25, 25);
}

void Visualization::Update(double elapsed_seconds) {
    Matrix rotation = Matrix::RotationYawPitchRoll(0.0, 0.0, 0.05);
    renderCamera.Position = Vector3::TransformCoordinate(renderCamera.Position, rotation);
    renderCamera.Light = Vector3::TransformCoordinate(renderCamera.Light, rotation);
}

void Visualization::Render(Device& renderDevice) {
    renderDevice.Clear(Color4(1.0, 1.0, 1.0, 1.0));

    renderDevice.RenderSurface(renderCamera, dodecahedron);
    //renderDevice.RenderWireframe(renderCamera, dodecahedron);
}
