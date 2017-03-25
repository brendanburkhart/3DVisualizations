#pragma once

#include "Vector4.h"
#include "Matrix.h"

class Vector3 {

public:
    double X, Y, Z;

    Vector3 ();

    Vector3 (double x, double y, double z);

    static Vector3 UnitY ();

    static Vector3 Subtract (Vector3 minuend, Vector3 subtrahend);

    static Vector3 Add (Vector3 summand1, Vector3 summand2);

    static Vector3 Multiply (Vector3 multiplicand, double multiplier);

    static Vector3 Divide (Vector3 dividend, double divisor);

    static Vector3 Negate (Vector3 vector);

    static Vector3 Normalize (Vector3 vector);

    static double Length (Vector3 vector);

    static double Dot (Vector3 left, Vector3 right);

    static Vector3 Cross (Vector3 left, Vector3 right);

    static Vector3 TransformCoordinate (Vector3 coordinate, Matrix transform);
};