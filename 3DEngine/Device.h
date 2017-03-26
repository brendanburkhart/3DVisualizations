#pragma once

#include "Vecor2.h"
#include "Vector3.h"
#include "Matrix.h"
#include "BackBuffer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Color4.h"

#include <vector>

class Device {
private: 
    // Implementation of byte [] as a back buffer for rendering
    BackBuffer backBuffer;
    
    int deviceWidth, deviceHeight;

public:
    Device ();

    Device (int pixelWidth, int pixelHeight);

    void Release ();
    
    // This method is called to clear the back buffer with a specific color
    void Clear (Color4 fillColor);

    // Once everything is ready, we can flush the back buffer
    // into the front buffer. 
    BackBuffer GetBuffer () const;

    // Called to put a pixel on screen at a specific X,Y coordinates
    void PutPixel (int x, int y, Color4 color);

    // Project takes some 3D coordinates and transform them
    // in 2D coordinates using the transformation matrix
    Vector2 Project (Vector3 coord, Matrix transMat);

    // DrawPoint calls PutPixel but does the clipping operation before
    void DrawPoint (Vector2 point);

    // The main method of the engine that re-compute each vertex projection
    // during each frame
    void Render (Camera camera, std::vector<Mesh> meshes);

    int getWidth ();
    int getHeight ();
};