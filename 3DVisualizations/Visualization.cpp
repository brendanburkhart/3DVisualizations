#include "stdafx.h"
#include "Visualization.h"

#include "ShapeMeshes.h"

Visualization::Visualization() {
    dodecahedron = ShapeMeshes::Dodecahedron();
    cube = ShapeMeshes::EmbeddedCube();

    wireframeOnly = false;

    renderCamera = Camera();
    renderCamera.Position = Vector3(25, 0, 0);
    renderCamera.Target = Vector3::Origin();
    renderCamera.Light = Vector3(25, 25, 25);
}

void Visualization::OnKeyDown(WPARAM wParam, LPARAM lParam) {
    switch (wParam) {
    case 'M':
        wireframeOnly = !wireframeOnly;
    }
}

void Visualization::Update(double elapsed_seconds) {
    double z = 0.0;
    double x = 0.0;

    if (GetKeyState(VK_LEFT) & 0x8000) {
        z -= 0.05;
    }

    if (GetKeyState(VK_RIGHT) & 0x8000) {
        z += 0.05;
    }

    if (GetKeyState(VK_UP) & 0x8000) {
        x -= 0.05;
    }

    if (GetKeyState(VK_DOWN) & 0x8000) {
        x += 0.05;
    }

    Matrix rotation = Matrix::RotationYawPitchRoll(x, 0.0, z);
    renderCamera.Position = Vector3::TransformCoordinate(renderCamera.Position, rotation);
    renderCamera.Light = Vector3::TransformCoordinate(renderCamera.Light, rotation);
}

void Visualization::Render(Device& renderDevice) {
    renderDevice.Clear(Color4(1.0, 1.0, 1.0, 1.0));

    if (wireframeOnly) {
        renderDevice.RenderWireframe(renderCamera, dodecahedron);
    }
    else {
        renderDevice.RenderSurface(renderCamera, dodecahedron);
    }
}
