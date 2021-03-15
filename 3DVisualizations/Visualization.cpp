#include "stdafx.h"
#include "Visualization.h"

#include <cmath>

#include "ShapeMeshes.h"

constexpr double PI = 3.14159265358979323846;

constexpr double dihedral_angle = 2.03444394; // 2*arctan(phi)
constexpr double align_angle1 = -1.2059325; // -arctan(phi^2)
constexpr double align_angle2 = -0.364863828; // -arctan(1/phi^2)
constexpr double align_angle3 = 0.556349562; // arcsin(1/sqrt(5-sqrt(2)))

Visualization::Visualization()
    : slerp(nullptr),
      viewRotation(Quaternion::EulerAngle(0.0, Vector3(1.0, 0.0, 0.0)))
{
    dodecahedron = ShapeMeshes::Dodecahedron();

    for (int i = 0; i < N; i++) {
        cubes[i] = ShapeMeshes::EmbeddedCube(i, false);
        largeCubes[i] = ShapeMeshes::EmbeddedCube(i, true);
        cubeToggles[i] = false;
    }

    wireframeOnly = false;
    fixWireframe = false;

    enlargeCubes = false;

    renderCamera = Camera();
    renderCamera.Position = Vector3(25, 0, 0);
    renderCamera.Target = Vector3::Origin();
    renderCamera.Light = Vector3(25, 15, 15);

    //resetView();
}

void Visualization::resetView() {
    slerp = nullptr;
    viewRotation = Quaternion::EulerAngle(
        0,
        Vector3(1.0, 0.0, 0.0)
    );
}

void Visualization::OnKeyDown(WPARAM wParam, LPARAM lParam) {
    Quaternion target = Quaternion::EulerAngle(0.0, Vector3(1.0, 0.0, 0.0));
    Quaternion delta = Quaternion::EulerAngle(0.0, Vector3(1.0, 0.0, 0.0));

    switch (wParam) {
    case 'M':
        wireframeOnly = !wireframeOnly;
        break;
    case 'L':
        enlargeCubes = !enlargeCubes;
        break;
    case '1':
        cubeToggles[0] = !cubeToggles[0];
        break;
    case '2':
        cubeToggles[1] = !cubeToggles[1];
        break;
    case '3':
        cubeToggles[2] = !cubeToggles[2];
        break;
    case '4':
        cubeToggles[3] = !cubeToggles[3];
        break;
    case '5':
        cubeToggles[4] = !cubeToggles[4];
        break;
    case 'A':
        if (slerp) break;
        target = Quaternion::EulerAngle(
            align_angle1,
            Vector3(0.0, 1.0, 0.0)
        );
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case 'B':
        if (slerp) break;
        target = Quaternion::EulerAngle(
            PI,
            Vector3(1.0, 0.0, 0.0)
        ).Multiply(Quaternion::EulerAngle(
            align_angle2,
            Vector3(0.0, 1.0, 0.0)
        )).Multiply(Quaternion::EulerAngle(-0.5 * PI, Vector3(0.0, 0.0, 1.0)));
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case 'C':
        if (slerp) break;
        target = Quaternion::EulerAngle(
            -align_angle2,
            Vector3(0.0, 1.0, 0.0)
        ).Multiply(Quaternion::EulerAngle(-0.5 * PI, Vector3(0.0, 0.0, 1.0)));
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case 'D':
        if (slerp) break;
        target = Quaternion::EulerAngle(-align_angle3, Vector3(0.0, 1.0, 0.0))
            .Multiply(Quaternion::EulerAngle(PI / 10.0, Vector3(1.0, 0.0, 0.0)))
            .Multiply(Quaternion::EulerAngle(0.5*PI - align_angle3, Vector3(0.0, 0.0, 1.0)));
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case 'E':
        if (slerp) break;
        target = Quaternion::EulerAngle(PI / 10.0, Vector3(1.0, 0.0, 0.0))
            .Multiply(Quaternion::EulerAngle(0.5 * PI - align_angle3, Vector3(0.0, 0.0, 1.0)));
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case 'S':
        // rotate by 2pi/3 around x-axis
        if (slerp) break;
        delta = Quaternion::EulerAngle(
            2.0*PI/3.0,
            Vector3(1.0, 0.0, 0.0)
        );
        target = delta.Multiply(viewRotation);
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case 'F':
        // rotate by pi around x-axis
        if (slerp) break;
        delta = Quaternion::EulerAngle(
            PI,
            Vector3(1.0, 0.0, 0.0)
        );
        target = delta.Multiply(viewRotation);
        slerp = std::make_unique<Slerp>(viewRotation, target, 1.0);
        break;
    case 'P':
        // rotate by 2*pi/5 around x-axis
        if (slerp) break;
        delta = Quaternion::EulerAngle(
            0.4 * PI,
            Vector3(1.0, 0.0, 0.0)
        );
        target = delta.Multiply(viewRotation);
        slerp = std::make_unique<Slerp>(viewRotation, target, 0.35);
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

    for (int i = 0; i < N; i++) {
        if (!cubeToggles[i]) continue;

        /*if (cubeWireframe) {
            Color4 color = Color4(
                std::fmod(0.25 * (i + 1), 1.0),
                std::fmod(0.50 * (i + 1), 1.0),
                std::fmod(0.75 * (i + 1), 1.0),
                1.0
            );
            renderDevice.RenderWireframe(renderCamera, cubes[i], rotation, color);
        }
        else {*/
            if (enlargeCubes) {
                renderDevice.RenderSurface(renderCamera, largeCubes[i], viewRotation);
            }
            else {
                renderDevice.RenderSurface(renderCamera, cubes[i], viewRotation);
            }
        //}
    }

    if (wireframeOnly || fixWireframe) {

        renderDevice.RenderWireframe(renderCamera, dodecahedron, rotation, Color4(0.0, 0.0, 0.0, 1.0), 3);
    }
}
