#pragma once
#include <jni.h>
#include <string>

struct Vector3 {
    double x, y, z;
    Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
    Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
    double Distance(const Vector3& other) const { return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2)); }
};

struct Vector2 { float x, y; };

struct Matrix4x4 { float m[4][4]; };

namespace Offsets {
    extern const char* MINECRAFT_CLIENT_CLASS;
    extern const char* GET_INSTANCE_METHOD;
    extern const char* PLAYER_FIELD;
    extern const char* WORLD_FIELD;
    extern const char* ENTITY_CLASS;
    extern const char* POS_X_FIELD;
    extern const char* POS_Y_FIELD;
    extern const char* POS_Z_FIELD;
    extern const char* HEALTH_FIELD;
    extern const char* BOUNDING_BOX_FIELD;
    extern const char* CLIENT_PLAYER_ENTITY_CLASS;
    extern const char* YAW_FIELD;
    extern const char* PITCH_FIELD;
    extern const char* GAME_RENDERER_CLASS;

    void InitializeOffsets(JNIEnv* env);
}
