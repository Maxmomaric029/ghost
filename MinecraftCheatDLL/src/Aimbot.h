#pragma once
#include "EntityCache.h"
#include <jni.h>

class Aimbot {
public:
    Aimbot();
    void Run(EntityCache& cache, JNIEnv* env, jobject localPlayer);

private:
    float m_fov = 10.0f;
    float m_smooth = 0.2f;
    float m_range = 100.0f;
    bool m_autoShoot = false;
    bool m_bowPrediction = true;
};
