#include "Aimbot.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"
#include "Utils.h"
#include <algorithm>

Aimbot::Aimbot() : m_smooth(0.15f), m_fov(45.0f), m_range(6.0f) {}

void Aimbot::Run(JNIEnv* env, jobject player) {
    if (!((GetKeyState(VK_LBUTTON) & 0x8000) != 0)) return;

    Vector3 lpPos;
    lpPos.x = env->GetDoubleField(player, MinecraftOffsets::g_PosXFieldID);
    lpPos.y = env->GetDoubleField(player, MinecraftOffsets::g_PosYFieldID);
    lpPos.z = env->GetDoubleField(player, MinecraftOffsets::g_PosZFieldID);

    float currentYaw = env->GetFloatField(player, MinecraftOffsets::g_YawFieldID);
    float currentPitch = env->GetFloatField(player, MinecraftOffsets::g_PitchFieldID);

    auto entities = CheatCore::Instance().GetCache().GetEntities();
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
        // Predecir posición futura (aprox 2 ticks adelante para compensar latencia/ping)
        float predictionScale = 0.15f; 
        Vector3 predictedPos = {
            (double)(bestTarget->pos.x + bestTarget->velocity.x * predictionScale),
            (double)(bestTarget->pos.y + bestTarget->velocity.y * predictionScale),
            (double)(bestTarget->pos.z + bestTarget->velocity.z * predictionScale)
        };

        double diffX = predictedPos.x - lpPos.x;
        double diffY = (predictedPos.y + 1.2) - (lpPos.y + 1.62);
        double diffZ = predictedPos.z - lpPos.z;
        double diffXZ = sqrt(diffX * diffX + diffZ * diffZ);

        float targetYaw = (float)Utils::RadianToDegree(atan2(diffZ, diffX)) - 90.0f;
        float targetPitch = (float)-Utils::RadianToDegree(atan2(diffY, diffXZ));

        float newYaw = Utils::Lerp(m_smooth, currentYaw, targetYaw);
        float newPitch = Utils::Lerp(m_smooth, currentPitch, targetPitch);
        
        newPitch = std::clamp(newPitch, -90.0f, 90.0f);

        env->SetFloatField(player, MinecraftOffsets::g_YawFieldID, newYaw);
        env->SetFloatField(player, MinecraftOffsets::g_PitchFieldID, newPitch);
    }
}
