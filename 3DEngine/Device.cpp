#include "stdafx.h"
#include "Device.h"

Device::Device () : deviceWidth(0), deviceHeight(0),backBuffer (BackBuffer ()) {}

Device::Device (int pixelWidth, int pixelHeight) : deviceWidth (pixelWidth), deviceHeight (pixelHeight) {
    backBuffer = BackBuffer (pixelWidth, pixelHeight);
}

void Device::Release () {
    backBuffer.Release ();
}

void Device::Clear (Color4 fillColor) {

    // Buffer data is in 4 byte-per-pixel format, iterates from 0 to end of buffer
    for (auto index = 0; index < (backBuffer.width * backBuffer.height * 4); index += 4) {
        // BGRA is the color system used by Windows.
        backBuffer [index] = (char)(fillColor.Blue * 255);
        backBuffer [index + 1] = (char)(fillColor.Green * 255);
        backBuffer [index + 2] = (char)(fillColor.Red * 255);
        backBuffer [index + 3] = (char)(fillColor.Alpha * 255);
    }
}

BackBuffer Device::GetBuffer () const{
    return backBuffer;
}

void Device::Render (Camera camera, std::vector<Mesh> meshes) {
    Matrix viewMatrix = Matrix::LookAtLH (camera.Position, camera.Target, Vector3::UnitY());
    Matrix projectionMatrix = Matrix::PerspectiveFovLH (0.78f,
        (double)deviceWidth / deviceHeight,
        0.01f, 1.0f);

    for (const auto &mesh : meshes) {
        // Make sure to apply rotation before translation 

        Matrix rotationMatrix = Matrix::RotationYawPitchRoll (mesh.Rotation.Y, mesh.Rotation.X, mesh.Rotation.Z);
        Matrix translationMatrix = Matrix::Translation (mesh.Position);

        Matrix worldMatrix = rotationMatrix * translationMatrix;

        Matrix transformMatrix = worldMatrix * viewMatrix * projectionMatrix;

        for (const auto &vertex : mesh.Vertices) {
            // First, we project the 3D coordinates into the 2D space
            auto point = Project (vertex, transformMatrix);
            // Then we can draw on screen
            DrawPoint (point);
        }
    }
}

Vector2 Device::Project (Vector3 coord, Matrix transMat) {
    // transforming the coordinates
    Vector2 point = Vector3::TransformCoordinate (coord, transMat);
    // The transformed coordinates will be based on coordinate system
    // starting on the center of the screen. But drawing on screen normally starts
    // from top left. We then need to transform them again to have x:0, y:0 on top left.
    double x = point.X * deviceWidth + deviceWidth / 2.0f;
    double y = -point.Y * deviceHeight + deviceHeight / 2.0f;
    return (Vector2 (x, y));
}

void Device::DrawPoint (Vector2 point) {
    // Clipping what's visible on screen
    if (point.X >= 0 && point.Y >= 0 && point.X < deviceWidth && point.Y < deviceHeight) {
        // Drawing a yellow point
        PutPixel ((int)point.X, (int)point.Y, Color4 (1.0f, 1.0f, 0.0f, 1.0f));
        OutputDebugString (L"SdfgKJ\n");
    } else {
        OutputDebugString (L"SGHKJ\n");
    }
}

void Device::PutPixel (int x, int y, Color4 color) {
    // As we have a 1-D Array for our back buffer
    // we need to know the equivalent cell in 1-D based
    // on the 2D coordinates on screen
    auto index = (x + y * backBuffer.scanLineSize);

    backBuffer [index] = (char)(color.Blue * 255);
    backBuffer [index + 1] = (char)(color.Green * 255);
    backBuffer [index + 2] = (char)(color.Red * 255);
    backBuffer [index + 3] = (char)(color.Alpha * 255);
}

int Device::getWidth () {
    return deviceWidth;
}

int Device::getHeight () {
    return deviceHeight;
}