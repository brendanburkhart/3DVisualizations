#include "stdafx.h"
#include "Visualization.h"

#include <cmath>

#include "ShapeMeshes.h"

Visualization::Visualization() {
    dodecahedron = ShapeMeshes::Dodecahedron();
    cube = ShapeMeshes::EmbeddedCube();

    wireframeOnly = false;

    renderCamera = Camera();
    renderCamera.Position = Vector3(25, 0, 0);
    renderCamera.Target = Vector3::Origin();
    renderCamera.Light = Vector3(25, 15, 15);

    resetView();
}

void Visualization::resetView() {
    viewRotation = Vector3(0.0, 0.0, 0.0);
}

void Visualization::OnKeyDown(WPARAM wParam, LPARAM lParam) {
    switch (wParam) {
    case 'M':
        wireframeOnly = !wireframeOnly;
        break;
    case 'R':
        resetView();
        break;
    }
}

void Visualization::Update(double elapsed_seconds) {
    double dx = 0.0;
    double dy = 0.0;
    double dz = 0.0;

    if (GetKeyState(VK_LEFT) & 0x8000) {
        dz -= 0.01;
    }

    if (GetKeyState(VK_RIGHT) & 0x8000) {
        dz += 0.01;
    }

    if (GetKeyState(VK_UP) & 0x8000) {
        dy -= 0.01;
    }

    if (GetKeyState(VK_DOWN) & 0x8000) {
        dy += 0.01;
    }

    viewRotation = Vector3(
        std::fmod(viewRotation.X + dx, 1.0),
        std::fmod(viewRotation.Y + dy, 1.0),
        std::fmod(viewRotation.Z + dz, 1.0)
    );
}

void Visualization::Render(Device& renderDevice) {
    renderDevice.Clear(Color4(1.0, 1.0, 1.0, 1.0));

    constexpr double PI = 3.14159265358979323846;

    Matrix rotationMatrix = Matrix::RotationYawPitchRoll(
        2.0 * PI *viewRotation.Y,
        2.0 * PI * viewRotation.X,
        2.0 * PI * viewRotation.Z
    );

    if (wireframeOnly) {
        renderDevice.RenderWireframe(renderCamera, dodecahedron, rotationMatrix);
    }
    else {
        renderDevice.RenderSurface(renderCamera, dodecahedron, rotationMatrix);
    }
}
