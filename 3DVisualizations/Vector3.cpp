#pragma once

#include "stdafx.h"

#include <cmath>

#include "Vector3.h"

Vector3::Vector3() {};

Vector3::Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {};

Vector3 Vector3::UnitY() {
    return Vector3(0.0, 1.0, 0.0);
}

Vector3 Vector3::Origin() {
    return Vector3(0.0, 0.0, 0.0);
}

Vector3 Vector3::Subtract(Vector3 minuend, Vector3 subtrahend) {
    return Vector3(minuend.X - subtrahend.X, minuend.Y - subtrahend.Y, minuend.Z - subtrahend.Z);
}

Vector3 Vector3::Add(Vector3 summand1, Vector3 summand2) {
    return Vector3(summand1.X + summand2.X, summand1.Y + summand2.Y, summand1.Z + summand2.Z);
}

Vector3 Vector3::Multiply(Vector3 multiplicand, double multiplier) {
    return Vector3(multiplicand.X * multiplier, multiplicand.Y * multiplier, multiplicand.Z * multiplier);
}

Vector3 Vector3::Divide(Vector3 dividend, double divisor) {
    return Vector3(dividend.X / divisor, dividend.Y / divisor, dividend.Z / divisor);
}

Vector3 Vector3::Negate(Vector3 vector) {
    return Vector3(-vector.X, -vector.Y, -vector.Z);
}

Vector3 Vector3::Normalize(Vector3 vector) {
    double length = Length(vector);

    if (!(length < 1e-6)) {
        double inverse = 1.0 / length;

        return Vector3(vector.X * inverse, vector.Y * inverse, vector.Z * inverse);
    }
    return vector;
}

double Vector3::Length(Vector3 vector) {
    return sqrt((vector.X * vector.X) + (vector.Y * vector.Y) + (vector.Z * vector.Z));
}

double Vector3::Dot(Vector3 left, Vector3 right) {
    return (left.X * right.X) + (left.Y * right.Y) + (left.Z * right.Z);
}

Vector3 Vector3::Cross(Vector3 left, Vector3 right) {
    return Vector3((left.Y * right.Z) - (left.Z * right.Y),

        (left.Z * right.X) - (left.X * right.Z),

        (left.X * right.Y) - (left.Y * right.X));
}

Vector3 Vector3::TransformCoordinate(Vector3 coordinate, Matrix transform) {
    Vector4 vector = Vector4(
        (coordinate.X * transform.data[0]) + (coordinate.Y * transform.data[4]) + (coordinate.Z * transform.data[8]) + transform.data[12],
        (coordinate.X * transform.data[1]) + (coordinate.Y * transform.data[5]) + (coordinate.Z * transform.data[9]) + transform.data[13],
        (coordinate.X * transform.data[2]) + (coordinate.Y * transform.data[6]) + (coordinate.Z * transform.data[10]) + transform.data[14],
        1.0 / ((coordinate.X * transform.data[3]) + (coordinate.Y * transform.data[7]) + (coordinate.Z * transform.data[11]) + transform.data[15])
    );
    return Vector3(vector.X * vector.W, vector.Y * vector.W, vector.Z * vector.W);
}
