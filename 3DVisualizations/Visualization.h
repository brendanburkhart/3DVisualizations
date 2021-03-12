#pragma once

#include "BackBuffer.h"
#include "Device.h"

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
    Mesh cube;

    bool wireframeOnly;
};
