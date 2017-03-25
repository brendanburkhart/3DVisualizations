#pragma once

#include "stdafx.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

#include <vector>
#include <math.h>

Matrix Matrix::Multiply (Matrix left, Matrix right) {
    Matrix returnMatrix = Matrix (0);

    returnMatrix.M11 = (left.M11 * right.M11) + (left.M12 * right.M21) + (left.M13 * right.M31) + (left.M14 * right.M41);
    returnMatrix.M12 = (left.M11 * right.M12) + (left.M12 * right.M22) + (left.M13 * right.M32) + (left.M14 * right.M42);
    returnMatrix.M13 = (left.M11 * right.M13) + (left.M12 * right.M23) + (left.M13 * right.M33) + (left.M14 * right.M43);
    returnMatrix.M14 = (left.M11 * right.M14) + (left.M12 * right.M24) + (left.M13 * right.M34) + (left.M14 * right.M44);
    returnMatrix.M21 = (left.M21 * right.M11) + (left.M22 * right.M21) + (left.M23 * right.M31) + (left.M24 * right.M41);
    returnMatrix.M22 = (left.M21 * right.M12) + (left.M22 * right.M22) + (left.M23 * right.M32) + (left.M24 * right.M42);
    returnMatrix.M23 = (left.M21 * right.M13) + (left.M22 * right.M23) + (left.M23 * right.M33) + (left.M24 * right.M43);
    returnMatrix.M24 = (left.M21 * right.M14) + (left.M22 * right.M24) + (left.M23 * right.M34) + (left.M24 * right.M44);
    returnMatrix.M31 = (left.M31 * right.M11) + (left.M32 * right.M21) + (left.M33 * right.M31) + (left.M34 * right.M41);
    returnMatrix.M32 = (left.M31 * right.M12) + (left.M32 * right.M22) + (left.M33 * right.M32) + (left.M34 * right.M42);
    returnMatrix.M33 = (left.M31 * right.M13) + (left.M32 * right.M23) + (left.M33 * right.M33) + (left.M34 * right.M43);
    returnMatrix.M34 = (left.M31 * right.M14) + (left.M32 * right.M24) + (left.M33 * right.M34) + (left.M34 * right.M44);
    returnMatrix.M41 = (left.M41 * right.M11) + (left.M42 * right.M21) + (left.M43 * right.M31) + (left.M44 * right.M41);
    returnMatrix.M42 = (left.M41 * right.M12) + (left.M42 * right.M22) + (left.M43 * right.M32) + (left.M44 * right.M42);
    returnMatrix.M43 = (left.M41 * right.M13) + (left.M42 * right.M23) + (left.M43 * right.M33) + (left.M44 * right.M43);
    returnMatrix.M44 = (left.M41 * right.M14) + (left.M42 * right.M24) + (left.M43 * right.M34) + (left.M44 * right.M44);

    return returnMatrix;
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