#pragma once

#include <array>
#include <memory>

#include "BackBuffer.h"
#include "Device.h"
#include "Slerp.h"
#include "Quaternion.h"

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

class Visualization
{
public:
    Visualization();

    void OnKeyDown(WPARAM wParam, LPARAM lParam);
    void Update(double elapsed_seconds);
    void Render(Device& renderDevice);

private:
    Camera renderCamera;
    Mesh dodecahedron;
    std::array<Mesh, 5> cubes;
    int n = 2;

    Quaternion viewRotation;

    bool wireframeOnly;
    bool fixWireframe;

    bool renderCube;
    bool cubeWireframe;

    std::unique_ptr<Slerp> slerp;

    void resetView();
};
