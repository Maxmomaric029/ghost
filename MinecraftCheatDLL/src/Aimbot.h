#pragma once
#include "EntityCache.h"
#include <jni.h>

class Aimbot {
public:
    Aimbot();
    void Run(EntityCache& cache, JNIEnv* env, jobject localPlayer);

private:
    float m_fov = 45.0f;
    float m_smooth = 0.15f;
    float m_range = 6.0f;
    bool m_autoShoot = false;
    bool m_bowPrediction = true;
};
