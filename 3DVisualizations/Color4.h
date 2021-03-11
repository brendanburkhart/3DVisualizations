#pragma once

#include "Vector3.h"

#include <algorithm>

struct Color4 {
    double Blue, Green, Red, Alpha;

    Color4(double blue, double green, double red, double alpha)
        : Blue(blue), Green(green), Red(red), Alpha(alpha) {}

    static Color4 Shade(Vector3 light, Vector3 normal, Color4 unshaded, double ambient) {
        double d = std::min(1.0, std::max(0.0, Vector3::Dot(normal, light)) + ambient);
        return Color4(unshaded.Blue * d, unshaded.Green * d, unshaded.Red * d, 1.0);
    }
};
