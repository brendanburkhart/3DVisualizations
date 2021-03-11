#include "stdafx.h"
#include "Device.h"

#include <algorithm>
#include <limits>

Device::Device()
    : deviceWidth(0), deviceHeight(0), depthBufferSize(0),
      backBuffer(BackBuffer()), depthBuffer(std::vector<double>()) {}

Device::Device(int pixelWidth, int pixelHeight)
    : deviceWidth(pixelWidth), deviceHeight(pixelHeight), depthBufferSize(pixelWidth * pixelHeight) {
    backBuffer = BackBuffer(pixelWidth, pixelHeight);
    depthBuffer.resize(depthBufferSize);
}

void Device::Clear(Color4 fillColor) {
    // Buffer data is in 4 byte-per-pixel format, iterates from 0 to end of buffer
    for (auto index = 0; index < (backBuffer.scanLineSize * backBuffer.height); index += 4) {
        // BGRA is the color system used by Windows.
        backBuffer[index] = (char)(fillColor.Blue * 255);
        backBuffer[index + 1] = (char)(fillColor.Green * 255);
        backBuffer[index + 2] = (char)(fillColor.Red * 255);
        backBuffer[index + 3] = (char)(fillColor.Alpha * 255);
    }

    // Clearing Depth Buffer
    for (auto index = 0; index < depthBufferSize; index++) {
        depthBuffer[index] = std::numeric_limits<double>::max();
    }
}

BackBuffer Device::GetBuffer() const {
    return backBuffer;
}

void Device::Render(const Camera& camera, const std::vector<Mesh>& meshes) {
    Matrix viewMatrix = Matrix::LookAtLH(camera.Position, camera.Target, Vector3::UnitY());
    Matrix projectionMatrix = Matrix::PerspectiveFovLH(0.78f,
        (double)deviceWidth / deviceHeight,
        0.01f, 1.0f);

    for (const auto& mesh : meshes) {
        // Make sure to apply rotation before translation 

        Matrix rotationMatrix = Matrix::RotationYawPitchRoll(mesh.Rotation.Y, mesh.Rotation.X, mesh.Rotation.Z);
        Matrix translationMatrix = Matrix::Translation(mesh.Position);

        Matrix worldMatrix = rotationMatrix * translationMatrix;

        Matrix transformMatrix = worldMatrix * viewMatrix * projectionMatrix;

        auto faceIndex = 0;
        for (const auto& face : mesh.Faces) {
            // Get each vertex for this face
            const auto& vertexA = mesh.Vertices[face.A];
            const auto& vertexB = mesh.Vertices[face.B];
            const auto& vertexC = mesh.Vertices[face.C];

            // Transform to get the pixel
            auto pixelA = Project(vertexA, transformMatrix);
            auto pixelB = Project(vertexB, transformMatrix);
            auto pixelC = Project(vertexC, transformMatrix);

            // Rasterize face as a triangles
            auto color = 0.25f + (faceIndex % mesh.Faces.size()) * 0.75f / mesh.Faces.size();
            RasterizeTriangle(pixelA, pixelB, pixelC, Color4(color, color, color, 1));
            faceIndex++;
        }
    }
}

Vector3 Device::Project(Vector3 coord, Matrix transMat) {
    // Transforming the coordinates
    Vector3 point = Vector3::TransformCoordinate(coord, transMat);

    // The transformed coordinates will be based on coordinate system
    // starting on the center of the screen. But drawing on screen normally starts
    // from top left. We then need to transform them again to have x:0, y:0 on top left.
    // Includes Z pos for the Z-Buffer, un-transformed
    double x = point.X * deviceWidth + deviceWidth / 2.0f;
    double y = -point.Y * deviceHeight + deviceHeight / 2.0f;
    return (Vector3(x, y, point.Z));
}

void Device::RasterizeTriangle(Vector3 p1, Vector3 p2, Vector3 p3, Color4 color) {
    // Sort points
    if (p1.Y > p2.Y) {
        auto temp = p2;
        p2 = p1;
        p1 = temp;
    }
    if (p2.Y > p3.Y) {
        auto temp = p2;
        p2 = p3;
        p3 = temp;
    }
    if (p1.Y > p2.Y) {
        auto temp = p2;
        p2 = p1;
        p1 = temp;
    }

    // Calculate inverse slopes
    double dP1P2, dP1P3;

    if (p2.Y - p1.Y > 0)
        dP1P2 = (p2.X - p1.X) / (p2.Y - p1.Y);
    else
        dP1P2 = 0;

    if (p3.Y - p1.Y > 0)
        dP1P3 = (p3.X - p1.X) / (p3.Y - p1.Y);
    else
        dP1P3 = 0;

    // Two cases for triangle shape once points are sorted
    if (dP1P2 > dP1P3) {
        // Iterate over height of triangle
        for (auto y = (int)p1.Y; y <= (int)p3.Y; y++) {
            // Reverse once second point is reached
            if (y < p2.Y) {
                // Draw line across triangle's width
                ProcessScanLine(y, p1, p3, p1, p2, color);
            }
            else {
                ProcessScanLine(y, p1, p3, p2, p3, color);
            }
        }
    }
    else {
        for (auto y = (int)p1.Y; y <= (int)p3.Y; y++) {
            if (y < p2.Y) {
                ProcessScanLine(y, p1, p2, p1, p3, color);
            }
            else {
                ProcessScanLine(y, p2, p3, p1, p3, color);
            }
        }
    }
}

double Device::Clamp(double value, double min, double max) {
    return std::max(min, std::min(value, max));
}

double Device::Interpolate(double min, double max, double gradient) {
    return min + ((max - min) * gradient);
}

void Device::ProcessScanLine(int y, Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, Color4 color) {
    auto gradient1 = pa.Y != pb.Y ? (y - pa.Y) / (pb.Y - pa.Y) : 1;
    auto gradient2 = pc.Y != pd.Y ? (y - pc.Y) / (pd.Y - pc.Y) : 1;

    int sx = (int)Interpolate(pa.X, pb.X, gradient1);
    int ex = (int)Interpolate(pc.X, pd.X, gradient2);

    double z1 = Interpolate(pa.Z, pb.Z, gradient1);
    double z2 = Interpolate(pc.Z, pd.Z, gradient2);

    // drawing a line from left (sx) to right (ex)
    for (auto x = sx; x < ex; x++) {
        double gradient = (x - sx) / (double)(ex - sx);

        auto z = Interpolate(z1, z2, gradient);
        DrawPoint(Vector3(x, y, z), color);
    }
}

void Device::DrawPoint(Vector3 point, Color4 color) {
    // Clip to device size
    if (point.X < 0 || point.Y < 0 || point.X >= deviceWidth || point.Y >= deviceHeight) {
        return;
    }

    auto index = (int)point.X + ((int)point.Y * deviceWidth);

    if (point.Z > depthBuffer[index]) {
        return; // Discard
    }

    depthBuffer[index] = point.Z;

    PutPixel((int)point.X, (int)point.Y, color);
}

void Device::PutPixel(int x, int y, Color4 color) {
    // As we have a 1-D Array for our back buffer
    // we need to know the equivalent cell in 1-D based
    // on the 2D coordinates on screen
    auto index = ((x * 4) + y * (backBuffer.scanLineSize));

    backBuffer[index] = (char)(color.Blue * 255);
    backBuffer[index + 1] = (char)(color.Green * 255);
    backBuffer[index + 2] = (char)(color.Red * 255);
    backBuffer[index + 3] = (char)(color.Alpha * 255);
}

int Device::getWidth() {
    return deviceWidth;
}

int Device::getHeight() {
    return deviceHeight;
}
