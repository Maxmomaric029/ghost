#pragma once
#include <cmath>
#include <algorithm>

namespace Utils {
    inline const double PI = 3.14159265358979323846;

    inline double DegreeToRadian(double degree) {
        return degree * (PI / 180.0);
    }

    inline double RadianToDegree(double radian) {
        return radian * (180.0 / PI);
    }

    inline float Clamp(float n, float lower, float upper) {
        return std::max(lower, std::min(n, upper));
    }

    inline float Lerp(float delta, float start, float end) {
        return start + delta * (end - start);
    }

    inline float NormalizeYaw(float yaw) {
        while (yaw > 180.0f) yaw -= 360.0f;
        while (yaw < -180.0f) yaw += 360.0f;
        return yaw;
    }

    inline float NormalizePitch(float pitch) {
        if (pitch > 90.0f) pitch = 90.0f;
        if (pitch < -90.0f) pitch = -90.0f;
        return pitch;
    }
}
