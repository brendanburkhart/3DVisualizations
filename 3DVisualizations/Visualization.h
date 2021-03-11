#pragma once

#include "BackBuffer.h"
#include "Device.h"

class Visualization
{
public:
    Visualization();

    void Update(LONGLONG elapsed_time);
    void Render(Device& renderDevice);

private:
    Camera renderCamera;
    std::vector<Mesh> meshes;
};
