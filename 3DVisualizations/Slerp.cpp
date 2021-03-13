#include "stdafx.h"
#include "Slerp.h"

#include <algorithm>
#include <cmath>

constexpr double epsilon = 0.00001;

Slerp::Slerp(Quaternion start, Quaternion end, double duration)
    : start(start), end(end), duration(duration), progress(0.0) {

    cosHalfTheta = start.r * end.r + start.a * end.a + start.b * end.b + start.c * end.c;
    cosHalfTheta = std::max(0.0, std::min(1.0, cosHalfTheta));

    halfTheta = acos(cosHalfTheta);
    sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
}

bool Slerp::Update(double elapsed_time) {
    progress += elapsed_time;

    if (progress >= duration) {
        progress = duration;
        return true;
    }

    return false;
}

Quaternion Slerp::Current() {
    // q_1 = q_2 or q_1 = -q_2, same rotation either way
    if (abs(cosHalfTheta) >= 1.0) {
        return Quaternion(start.r, start.a, start.b, start.c);
    }

    // Theta = 180, so rotation is through 360 degrees
    if (abs(sinHalfTheta) < epsilon) {
        return Quaternion(
            0.5 * (start.r + end.r),
            0.5 * (start.a + end.a),
            0.5 * (start.b + end.b),
            0.5 * (start.c + end.c)
        );
    }

    double t = progress / duration;
    double ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
    double ratioB = sin(t * halfTheta) / sinHalfTheta;

    return Quaternion(
        ratioA * start.r + ratioB * end.r,
        ratioA * start.a + ratioB * end.a,
        ratioA * start.b + ratioB * end.b,
        ratioA * start.c + ratioB * end.c
    );
}
