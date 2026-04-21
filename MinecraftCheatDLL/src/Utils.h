#pragma once
#include <cmath>

struct Vector2 { float x, y; };
struct Vector3 { 
    double x, y, z; 
    double Distance(const Vector3& other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));
    }
};
struct Matrix4x4 { float m[4][4]; };

namespace Utils {
    const double PI = 3.14159265358979323846;

    inline float ToDegrees(float rad) { return rad * (float)(180.0 / PI); }
    inline float ToRadians(float deg) { return deg * (float)(PI / 180.0); }
    inline double RadianToDegree(double rad) { return rad * (180.0 / PI); }
    
    inline float Lerp(float delta, float start, float end) {
        return start + delta * (end - start);
    }

    inline float AngleDiff(float a, float b) {
        float diff = fmodf(b - a + 180.0f, 360.0f) - 180.0f;
        return diff < -180.0f ? diff + 360.0f : diff;
    }
}
