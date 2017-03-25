#pragma once

#include "Vector3.h"

class Vector2 {
public:
    double X, Y;

    Vector2 (double x, double y);

    Vector2 (Vector3 vector);
};