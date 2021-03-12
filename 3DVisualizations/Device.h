#pragma once

#include "Vector3.h"
#include "BackBuffer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Color4.h"

#include <vector>

class Device {
private:
    BackBuffer backBuffer;
    std::vector<double> depthBuffer;

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

    // Draws a line between point A and point B using Bresenham's algorithm
    void DrawLine(Vector3 pointA, Vector3 pointB, Color4 color);

    // DrawPoint calls PutPixel but does the clipping operation before
    void DrawPoint(Vector3 point, Color4 color);

    // Called to put a pixel on screen at a specific X,Y coordinates
    void PutPixel(int x, int y, Color4 color);

public:
    Device();
    Device(int pixelWidth, int pixelHeight);

    // This method is called to clear the back buffer with a specific color
    void Clear(Color4 fillColor);

    // Once everything is ready, we can flush the back buffer
    // into the front buffer. 
    BackBuffer GetBuffer() const;

    void RenderSurface(const Camera& camera, const Mesh& mesh);
    void RenderWireframe(const Camera& camera, const Mesh& mesh);

    int getWidth();
    int getHeight();
};
