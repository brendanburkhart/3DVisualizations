#pragma once

class Vector3;

class Matrix {
    double data[4 * 4];

public:

    Matrix (double value) {
        for (int i = 0; i < 4 * 4; i++) {
            data[i] = value;
        }
    }

    Matrix (double M11, double M12, double M13, double M14,
        double M21, double M22, double M23, double M24,
        double M31, double M32, double M33, double M34,
        double M41, double M42, double M43, double M44) {
        data[0] = M11;
        data[1] = M12;
        data[2] = M13;
        data[3] = M14;
        data[4] = M21;
        data[5] = M22;
        data[6] = M23;
        data[7] = M24;
        data[8] = M31;
        data[9] = M32;
        data[10] = M33;
        data[11] = M34;
        data[12] = M41;
        data[13] = M42;
        data[14] = M43;
        data[15] = M44;
    }

    static Matrix Multiply (const Matrix& a, const Matrix& b);

    static Matrix LookAtLH (Vector3 eye, Vector3 target, Vector3 up);

    static Matrix PerspectiveFovLH (double fov, double aspectRatio, double zNear, double zFar);

    static Matrix RotationYawPitchRoll (double yRotation, double xRotation, double zRotation);

    static Matrix Translation (Vector3 translation);

    Matrix operator * (const Matrix& multiplier);

    friend Vector3;
};