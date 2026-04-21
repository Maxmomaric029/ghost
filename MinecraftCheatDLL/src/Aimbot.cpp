#include "Aimbot.h"
#include <cmath>

Aimbot::Aimbot() {}

void Aimbot::Run(const EntityCache& cache, JNIEnv* env, jobject localPlayer) {
    if (!GetAsyncKeyState(VK_LBUTTON)) return;

    const CachedEntity* bestTarget = nullptr;
    float bestFov = m_fov;

    jclass entityClass = JVMHelper::FindClassCached(env, Offsets::ENTITY_CLASS);
    jfieldID xFid = env->GetFieldID(entityClass, Offsets::POS_X_FIELD, "D");
    jfieldID yFid = env->GetFieldID(entityClass, Offsets::POS_Y_FIELD, "D");
    jfieldID zFid = env->GetFieldID(entityClass, Offsets::POS_Z_FIELD, "D");

    Vector3 lpPos = { env->GetDoubleField(localPlayer, xFid), env->GetDoubleField(localPlayer, yFid), env->GetDoubleField(localPlayer, zFid) };

    for (const auto& entity : cache.GetEntities()) {
        float dist = (float)lpPos.Distance(entity.position);
        if (dist > m_range) continue;

        // Calculate angles and FOV check
        bestTarget = &entity;
    }

    if (bestTarget) {
        jclass lpClass = JVMHelper::FindClassCached(env, Offsets::CLIENT_PLAYER_ENTITY_CLASS);
        jfieldID yawFid = env->GetFieldID(lpClass, Offsets::YAW_FIELD, "F");
        jfieldID pitchFid = env->GetFieldID(lpClass, Offsets::PITCH_FIELD, "F");

        float currentYaw = env->GetFloatField(localPlayer, yawFid);
        float currentPitch = env->GetFloatField(localPlayer, pitchFid);

        // Smooth rotation logic
        float newYaw = currentYaw + 1.0f; // Placeholder for calculated target angle
        env->SetFloatField(localPlayer, yawFid, newYaw);
    }
}
