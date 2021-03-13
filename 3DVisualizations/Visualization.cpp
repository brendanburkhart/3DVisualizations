#include "stdafx.h"
#include "Visualization.h"

#include <cmath>

#include "ShapeMeshes.h"

constexpr double PI = 3.14159265358979323846;

Visualization::Visualization()
    : slerp(nullptr),
      viewRotation(Quaternion::EulerAngle(0.0, Vector3(1.0, 0.0, 0.0)))
{
    dodecahedron = ShapeMeshes::Dodecahedron();
    cubes[0] = ShapeMeshes::EmbeddedCube(0);
    cubes[1] = ShapeMeshes::EmbeddedCube(1);

    wireframeOnly = false;
    fixWireframe = false;

    renderCube = false;
    cubeWireframe = false;

    renderCamera = Camera();
    renderCamera.Position = Vector3(25, 0, 0);
    renderCamera.Target = Vector3::Origin();
    renderCamera.Light = Vector3(25, 15, 15);

    resetView();
}

void Visualization::resetView() {
    slerp = nullptr;
    viewRotation = Quaternion::EulerAngle(0.0, Vector3(1.0, 0.0, 0.0));
}

void Visualization::OnKeyDown(WPARAM wParam, LPARAM lParam) {
    Quaternion target = Quaternion::EulerAngle(0.0, Vector3(1.0, 0.0, 0.0));
    Quaternion delta = Quaternion::EulerAngle(0.0, Vector3(1.0, 0.0, 0.0));

    constexpr double dihedral_angle = 2.03444394; // 2*arctan(phi)
    constexpr double align_angle = -1.2059325;

    switch (wParam) {
    case 'M':
        wireframeOnly = !wireframeOnly;
        break;
    case 'F':
        fixWireframe = !fixWireframe;
        break;
    case 'C':
        renderCube = !renderCube;
        break;
    case 'N':
        cubeWireframe = !cubeWireframe;
        break;
    case 'S':
        // rotate by (PI - dihedral angle) around y-axis
        if (slerp) break;
        target = Quaternion::EulerAngle(
            align_angle,
            Vector3(0.0, 1.0, 0.0)
        );
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case '1':
        // rotate by 2pi/3 around x-axis
        if (slerp) break;
        delta = Quaternion::EulerAngle(
            2.0*PI/3.0,
            Vector3(1.0, 0.0, 0.0)
        );
        target = delta.Multiply(viewRotation);
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case 'R':
        resetView();
        break;
    }
}

void Visualization::Update(double elapsed_seconds) {
    double ax = 0.0;
    double ay = 0.0;
    double az = 0.0;
    double theta = 0.0;

    if (GetKeyState(VK_LEFT) & 0x8000) {
        az = 1.0;
        theta = 0.05;
    }

    if (GetKeyState(VK_RIGHT) & 0x8000) {
        az = -1.0;
        theta = 0.05;
    }

    if (GetKeyState(VK_UP) & 0x8000) {
        ay = -1.0;
        theta = 0.05;
    }

    if (GetKeyState(VK_DOWN) & 0x8000) {
        ay = 1.0;
        theta = 0.05;
    }

    if (slerp) {
        bool done = slerp->Update(elapsed_seconds);
        viewRotation = slerp->Current();
        if (done) {
            slerp = nullptr;
        }
    }
    else {
        Quaternion delta = Quaternion::EulerAngle(theta, Vector3(ax, ay, az));
        viewRotation = delta.Multiply(viewRotation);
    }
}

void Visualization::Render(Device& renderDevice) {
    renderDevice.Clear(Color4(1.0, 1.0, 1.0, 1.0));
    
    if (!wireframeOnly) {
        renderDevice.RenderSurface(renderCamera, dodecahedron, viewRotation);
    }

    Quaternion rotation = viewRotation;
    if (fixWireframe) {
        rotation = Quaternion::EulerAngle(0.0, Vector3(1.0, 0.0, 0.0));
    }

    if (wireframeOnly || fixWireframe) {
        
        renderDevice.RenderWireframe(renderCamera, dodecahedron, rotation, Color4(0.0, 0.0, 0.0, 1.0));
    }

    if (renderCube) {
        if (cubeWireframe) {
            for (int i = 0; i < n; i++) {
                Color4 color = Color4(
                    std::fmod(0.25 * (i + 1), 1.0),
                    std::fmod(0.50 * (i + 1), 1.0),
                    std::fmod(0.75 * (i + 1), 1.0),
                    1.0
                );
                renderDevice.RenderWireframe(renderCamera, cubes[i], rotation, color);
            }
        }
        else {
            renderDevice.RenderSurface(renderCamera, cubes[0], viewRotation);
        }
    }
}
