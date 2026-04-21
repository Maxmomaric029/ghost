#pragma once
#include <windows.h>
#include <cmath>
#include <algorithm>

struct Vector2 { float x, y; };
struct Vector3 { 
    double x, y, z; 
    Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
    double Length() const { return std::sqrt(x * x + y * y + z * z); }
    double Distance(const Vector3& other) const { return (*this - other).Length(); }
};

struct Matrix4x4 { 
    float m[4][4]; 
    
    // Multiplicación para proyección
    Vector3 Multiply(const Vector3& v, float& w) const {
        w = (float)v.x * m[0][3] + (float)v.y * m[1][3] + (float)v.z * m[2][3] + m[3][3];
        return {
            (v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0]),
            (v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1]),
            (v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2])
        };
    }
};

namespace Utils {
    inline const double PI = 3.14159265358979323846;

    // Normalización para evitar el latigazo al cruzar el límite de -180/180
    inline float NormalizeYaw(float yaw) {
        while (yaw > 180.0f) yaw -= 360.0f;
        while (yaw < -180.0f) yaw += 360.0f;
        return yaw;
    }

    // Diferencia angular circular correcta
    inline float AngleDiff(float current, float target) {
        float diff = target - current;
        return NormalizeYaw(diff);
    }

    // Interpolación lineal sobre diferencia angular normalizada
    inline float Lerp(float delta, float start, float end) {
        return start + delta * AngleDiff(start, end);
    }

    /**
     * WorldToScreen: Proyecta coordenadas 3D a 2D usando matrices de cámara.
     */
    bool WorldToScreen(const Vector3& worldPos, const Matrix4x4& viewMatrix, const Matrix4x4& projMatrix, int width, int height, Vector2& screenPos);

    inline double DegreeToRadian(double deg) { return deg * (PI / 180.0); }
    inline double RadianToDegree(double rad) { return rad * (180.0 / PI); }
}
