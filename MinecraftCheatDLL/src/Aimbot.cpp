#include "Aimbot.h"
#include "MinecraftOffsets.h"
#include "Utils.h"
#include <cmath>

Aimbot::Aimbot() {}

void Aimbot::Run(EntityCache& cache, JNIEnv* env, jobject localPlayer) {
    if (!(GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))) return;

    auto entities = cache.GetEntities();
    const CachedEntity* bestTarget = nullptr;
    float bestFov = m_fov;

    Vector3 lpPos = { 
        env->GetDoubleField(localPlayer, MinecraftOffsets::g_PosXFieldID),
        env->GetDoubleField(localPlayer, MinecraftOffsets::g_PosYFieldID),
        env->GetDoubleField(localPlayer, MinecraftOffsets::g_PosZFieldID)
    };

    float currentYaw = env->GetFloatField(localPlayer, MinecraftOffsets::g_YawFieldID);
    float currentPitch = env->GetFloatField(localPlayer, MinecraftOffsets::g_PitchFieldID);

    for (const auto& entity : entities) {
        if (entity.health <= 0) continue;
        
        float dist = (float)lpPos.Distance(entity.position);
        if (dist > m_range) continue;

        Vector3 targetPos = entity.headPos;
        double diffX = targetPos.x - lpPos.x;
        double diffY = targetPos.y - (lpPos.y + 1.62); // Eye height
        double diffZ = targetPos.z - lpPos.z;
        double diffXZ = sqrt(diffX * diffX + diffZ * diffZ);

        float targetYaw = (float)Utils::RadianToDegree(atan2(diffZ, diffX)) - 90.0f;
        float targetPitch = (float)-Utils::RadianToDegree(atan2(diffY, diffXZ));

        float fov = abs(Utils::AngleDiff(currentYaw, targetYaw)) + abs(Utils::AngleDiff(currentPitch, targetPitch));
        if (fov < bestFov) {
            bestFov = fov;
            bestTarget = &entity;
        }
    }

    if (bestTarget) {
        Vector3 targetPos = bestTarget->headPos;
        double diffX = targetPos.x - lpPos.x;
        double diffY = targetPos.y - (lpPos.y + 1.62);
        double diffZ = targetPos.z - lpPos.z;
        double diffXZ = sqrt(diffX * diffX + diffZ * diffZ);

        float targetYaw = (float)Utils::RadianToDegree(atan2(diffZ, diffX)) - 90.0f;
        float targetPitch = (float)-Utils::RadianToDegree(atan2(diffY, diffXZ));

        float smoothYaw = Utils::Lerp(m_smooth, currentYaw, targetYaw);
        float smoothPitch = Utils::Lerp(m_smooth, currentPitch, targetPitch);

        env->SetFloatField(localPlayer, MinecraftOffsets::g_YawFieldID, smoothYaw);
        env->SetFloatField(localPlayer, MinecraftOffsets::g_PitchFieldID, smoothPitch);
    }
}
