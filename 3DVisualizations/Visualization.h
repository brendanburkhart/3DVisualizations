#pragma once

#include "BackBuffer.h"
#include "Device.h"

class Visualization
{
public:
    Visualization();

    void Update(double elapsed_seconds);
    void Render(Device& renderDevice);

private:
    Camera renderCamera;
    std::vector<Mesh> meshes;
    std::vector<Wireframe> wireframes;
};
