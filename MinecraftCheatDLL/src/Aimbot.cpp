#include "Aimbot.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"
#include "Utils.h"
#include <algorithm>

Aimbot::Aimbot() : m_smooth(0.15f), m_fov(45.0f), m_range(6.0f) {}

void Aimbot::Run(EntityCache& cache, JNIEnv* env, jobject player) {
    if (!GetAsyncKeyState(VK_LBUTTON)) return;

    Vector3 lpPos;
    lpPos.x = env->GetDoubleField(player, MinecraftOffsets::g_PosXFieldID);
    lpPos.y = env->GetDoubleField(player, MinecraftOffsets::g_PosYFieldID);
    lpPos.z = env->GetDoubleField(player, MinecraftOffsets::g_PosZFieldID);

    float currentYaw = env->GetFloatField(player, MinecraftOffsets::g_YawFieldID);
    float currentPitch = env->GetFloatField(player, MinecraftOffsets::g_PitchFieldID);

    auto entities = cache.GetEntities();
    const CachedEntity* bestTarget = nullptr;
    float minFov = m_fov;

    for (const auto& entity : entities) {
        if (entity.health <= 0.0f) continue;

        float dist = (float)lpPos.Distance(entity.pos);
        if (dist > m_range) continue;

        double diffX = entity.pos.x - lpPos.x;
        double diffY = (entity.pos.y + 1.2) - (lpPos.y + 1.62);
        double diffZ = entity.pos.z - lpPos.z;
        double diffXZ = sqrt(diffX * diffX + diffZ * diffZ);

        float targetYaw = (float)Utils::RadianToDegree(atan2(diffZ, diffX)) - 90.0f;
        float targetPitch = (float)-Utils::RadianToDegree(atan2(diffY, diffXZ));

        float fovYaw = abs(Utils::AngleDiff(currentYaw, targetYaw));
        float fovPitch = abs(Utils::AngleDiff(currentPitch, targetPitch));

        if (fovYaw + fovPitch < minFov) {
            minFov = fovYaw + fovPitch;
            bestTarget = &entity;
        }
    }

    if (bestTarget) {
        double diffX = bestTarget->pos.x - lpPos.x;
        double diffY = (bestTarget->pos.y + 1.2) - (lpPos.y + 1.62);
        double diffZ = bestTarget->pos.z - lpPos.z;
        double diffXZ = sqrt(diffX * diffX + diffZ * diffZ);

        float targetYaw = (float)Utils::RadianToDegree(atan2(diffZ, diffX)) - 90.0f;
        float targetPitch = (float)-Utils::RadianToDegree(atan2(diffY, diffXZ));

        float newYaw = Utils::Lerp(m_smooth, currentYaw, targetYaw);
        float newPitch = Utils::Lerp(m_smooth, currentPitch, targetPitch);
        
        // CORRECCIÓN: Clamp de Pitch para evitar "cuello roto" o ban
        newPitch = std::clamp(newPitch, -90.0f, 90.0f);

        env->SetFloatField(player, MinecraftOffsets::g_YawFieldID, newYaw);
        env->SetFloatField(player, MinecraftOffsets::g_PitchFieldID, newPitch);
    }
}
