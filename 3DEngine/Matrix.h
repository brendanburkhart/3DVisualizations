#pragma once

class Vector3;

class Matrix {
public:
    double M11, M12, M13, M14,
           M21, M22, M23, M24,
           M31, M32, M33, M34,
           M41, M42, M43, M44;

    Matrix (double value) :
        M11 (value), M12 (value), M13 (value), M14 (value),
        M21 (value), M22 (value), M23 (value), M24 (value),
        M31 (value), M32 (value), M33 (value), M34 (value),
        M41 (value), M42 (value), M43 (value), M44 (value) {}

    Matrix (double M11, double M12, double M13, double M14,
        double M21, double M22, double M23, double M24,
        double M31, double M32, double M33, double M34,
        double M41, double M42, double M43, double M44) :

        M11 (M11), M12 (M12), M13 (M13), M14 (M14),
        M21 (M21), M22 (M22), M23 (M23), M24 (M24),
        M31 (M31), M32 (M32), M33 (M33), M34 (M34),
        M41 (M41), M42 (M42), M43 (M43), M44 (M44) {}

    static Matrix Multiply (Matrix multiplicand, Matrix multiplier);

    static Matrix LookAtLH (Vector3 eye, Vector3 target, Vector3 up);

    static Matrix PerspectiveFovLH (double fov, double aspectRatio, double zNear, double zFar);

    static Matrix RotationYawPitchRoll (double yRotation, double xRotation, double zRotation);

    static Matrix Translation (Vector3 translation);

    Matrix operator * (const Matrix& multiplier);
};