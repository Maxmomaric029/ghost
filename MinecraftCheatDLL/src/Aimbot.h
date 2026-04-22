#pragma once
#include "EntityCache.h"
#include <jni.h>

#include "IModule.h"

class Aimbot : public IModule {
public:
    Aimbot();
    void Run(JNIEnv* env, jobject localPlayer) override;
    const char* GetName() const override { return "Aimbot"; }

    float m_fov = 45.0f;
    float m_smooth = 0.15f;
    float m_range = 6.0f;
};
