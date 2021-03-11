#pragma once

#include "Vector3.h"
#include "BackBuffer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Color4.h"

#include <vector>

class Device {
private:
    // Implementation of byte [] as a back buffer for rendering
    BackBuffer backBuffer;

    double* depthBuffer;

    int deviceWidth, deviceHeight, depthBufferSize;

    // Project takes coordinates and transforms them
    // using the transformation matrix
    Vector3 Project(Vector3 coord, Matrix transMat);

    void RasterizeTriangle(Vector3 p1, Vector3 p2, Vector3 p3, Color4 color);

    // Draw scan line at y in triangle formed by pa, pb, and pc
    void ProcessScanLine(int y, Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, Color4 color);

    // Clamps value between min and max
    double Clamp(double value, double min = 0, double max = 1);

    // Interpolates gradient between min and max
    double Interpolate(double min, double max, double gradient);

    // DrawPoint calls PutPixel but does the clipping operation before
    void DrawPoint(Vector3 point, Color4 color);

    // Called to put a pixel on screen at a specific X,Y coordinates
    void PutPixel(int x, int y, Color4 color);

public:
    Device();

    Device(int pixelWidth, int pixelHeight);

    void Release();

    // This method is called to clear the back buffer with a specific color
    void Clear(Color4 fillColor);

    // Once everything is ready, we can flush the back buffer
    // into the front buffer. 
    BackBuffer GetBuffer() const;

    // The main method of the engine that re-compute each vertex projection
    // during each frame
    void Render(Camera camera, std::vector<Mesh> meshes);

    int getWidth();
    int getHeight();
};