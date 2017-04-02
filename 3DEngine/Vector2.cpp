#include "stdafx.h"

#include "Vecor2.h"

Vector2::Vector2 (double x, double y) : X (x), Y (y) {};

Vector2::Vector2 (Vector3 vector) : X (vector.X), Y (vector.Y) {}