#pragma once
#include <cmath>

namespace MathUtils {
    inline const double PI = 3.14159265358979323846;

    inline double ToDegrees(double radians) {
        return radians * (180.0 / PI);
    }

    inline double ToRadians(double degrees) {
        return degrees * (PI / 180.0);
    }
}
