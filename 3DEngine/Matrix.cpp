#pragma once

#include "stdafx.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

#include <vector>
#include <cmath>

Matrix Matrix::Multiply (const Matrix& a, const Matrix& b) {
    Matrix product = Matrix (0);

    for (int row = 0; row < 4; row++) {
        for (int i = 0; i < 4; i++) {
            const double a_row_i = a.data[row * 4 + i];

            for (int col = 0; col < 4; col++) {
                product.data[row * 4 + col] += a_row_i * b.data[i * 4 + col];
            }
        }
    }

    return product;
}

Matrix Matrix::LookAtLH (Vector3 eye, Vector3 target, Vector3 up) {
    Vector3 xAxis, yAxis, zAxis;

    zAxis = Vector3::Normalize (Vector3::Subtract(target, eye));
    xAxis = Vector3::Normalize (Vector3::Cross (up, zAxis));
    yAxis = Vector3::Cross (zAxis, xAxis);

    Matrix returnMatrix = Matrix (xAxis.X, yAxis.X, zAxis.X, 0.0,
        xAxis.Y, yAxis.Y, zAxis.Y, 0.0,
        xAxis.Z, yAxis.Z, zAxis.Z, 0.0,
        -Vector3::Dot(xAxis, eye), -Vector3::Dot (yAxis, eye), -Vector3::Dot (zAxis, eye), 1.0);

    return returnMatrix;
}

Matrix Matrix::PerspectiveFovLH (double fov, double aspectRatio, double zNear, double zFar) {

    double yScale = (1.0 / tan (fov * 0.5));
    double q = zFar / (zFar - zNear);

    Matrix returnMatrix = Matrix ((yScale / aspectRatio), 0.0, 0.0, 0.0,
        0.0, yScale, 0.0, 0.0,
        0.0, 0.0, q, 1.0,
        0.0, 0.0, -q * zNear, 0.0);

    return returnMatrix;
}

Matrix Matrix::RotationYawPitchRoll (double yaw, double pitch, double roll) {
    double halfRoll = roll * 0.5f;
    double halfPitch = pitch * 0.5f;
    double halfYaw = yaw * 0.5f;

    double sinRoll = (double)sin (halfRoll);
    double cosRoll = (double)cos (halfRoll);
    double sinPitch = (double)sin (halfPitch);
    double cosPitch = (double)cos (halfPitch);
    double sinYaw = (double)sin (halfYaw);
    double cosYaw = (double)cos (halfYaw);

    Vector4 rotation = Vector4 (
        (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll),
        (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll),
        (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll),
        (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll));

    double xx = rotation.X * rotation.X;
    double yy = rotation.Y * rotation.Y;
    double zz = rotation.Z * rotation.Z;
    double xy = rotation.X * rotation.Y;
    double zw = rotation.Z * rotation.W;
    double zx = rotation.Z * rotation.X;
    double yw = rotation.Y * rotation.W;
    double yz = rotation.Y * rotation.Z;
    double xw = rotation.X * rotation.W;

    Matrix returnMatrix = Matrix (
        1.0f - (2.0f * (yy + zz)), 2.0f * (xy + zw), 2.0f * (zx - yw), 0.0,
        2.0f * (xy - zw), 1.0f - (2.0f * (zz + xx)), 2.0f * (yz + xw), 0.0,
        2.0f * (zx + yw), 2.0f * (yz - xw), 1.0f - (2.0f * (yy + xx)), 0.0,
        0.0, 0.0, 0.0, 1.0);

    return returnMatrix;
}

Matrix Matrix::Translation (Vector3 translation) {
    return Matrix (
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        translation.X, translation.Y, translation.Z, 1
    );
}

Matrix Matrix::operator * (const Matrix& multiplier) {
    return Matrix::Multiply (*this, multiplier);
}