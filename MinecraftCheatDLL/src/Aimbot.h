#pragma once
#include "EntityCache.h"
#include "JVMHelper.h"

class Aimbot {
public:
    Aimbot();
    void Run(const EntityCache& cache, JNIEnv* env, jobject localPlayer);

private:
    float m_fov = 90.0f;
    float m_smooth = 0.5f;
    float m_range = 100.0f;
};
