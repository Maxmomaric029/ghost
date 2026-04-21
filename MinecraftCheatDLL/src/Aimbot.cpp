#include "Aimbot.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"
#include "Utils.h"

Aimbot::Aimbot() : m_smooth(0.15f), m_fov(45.0f), m_range(6.0f) {}

/**
 * Lógica principal del Aimbot.
 * Corrige el latigazo mediante normalización angular y Lerp circular.
 */
void Aimbot::Run(EntityCache& cache, JNIEnv* env, jobject player) {
    if (!GetAsyncKeyState(VK_LBUTTON)) return;

    jclass playerClass = env->GetObjectClass(player);
    ScopedLocalRef slrPlayerClass(env, playerClass);

    // Obtener rotaciones y posición actuales
    float currentYaw = env->GetFloatField(player, env->GetFieldID(playerClass, Offsets::FIELD_YAW, "F"));
    float currentPitch = env->GetFloatField(player, env->GetFieldID(playerClass, Offsets::FIELD_PITCH, "F"));

    Vector3 lpPos;
    lpPos.x = env->GetDoubleField(player, env->GetFieldID(playerClass, Offsets::FIELD_POS_X, "D"));
    lpPos.y = env->GetDoubleField(player, env->GetFieldID(playerClass, Offsets::FIELD_POS_Y, "D"));
    lpPos.z = env->GetDoubleField(player, env->GetFieldID(playerClass, Offsets::FIELD_POS_Z, "D"));

    auto entities = cache.GetEntities();
    const CachedData* bestTarget = nullptr;
    float minFov = m_fov;

    for (const auto& entity : entities) {
        if (entity.health <= 0.0f) continue;

        float dist = (float)lpPos.Distance(entity.pos);
        if (dist > m_range) continue;

        // Calcular ángulos necesarios
        double diffX = entity.pos.x - lpPos.x;
        double diffY = (entity.pos.y + 1.2) - (lpPos.y + 1.62); // Altura de ojos aprox
        double diffZ = entity.pos.z - lpPos.z;
        double diffXZ = sqrt(diffX * diffX + diffZ * diffZ);

        float targetYaw = (float)Utils::RadianToDegree(atan2(diffZ, diffX)) - 90.0f;
        float targetPitch = (float)-Utils::RadianToDegree(atan2(diffY, diffXZ));

        // Diferencia angular normalizada para evitar rotaciones bruscas
        float fovYaw = abs(Utils::AngleDiff(currentYaw, targetYaw));
        float fovPitch = abs(Utils::AngleDiff(currentPitch, targetPitch));

        if (fovYaw + fovPitch < minFov) {
            minFov = fovYaw + fovPitch;
            bestTarget = &entity;
        }
    }

    if (bestTarget) {
        // Recalcular para el objetivo seleccionado
        double diffX = bestTarget->pos.x - lpPos.x;
        double diffY = (bestTarget->pos.y + 1.2) - (lpPos.y + 1.62);
        double diffZ = bestTarget->pos.z - lpPos.z;
        double diffXZ = sqrt(diffX * diffX + diffZ * diffZ);

        float targetYaw = (float)Utils::RadianToDegree(atan2(diffZ, diffX)) - 90.0f;
        float targetPitch = (float)-Utils::RadianToDegree(atan2(diffY, diffXZ));

        // CORRECCIÓN: Suavizado aplicado sobre diferencia angular normalizada
        float newYaw = Utils::Lerp(m_smooth, currentYaw, targetYaw);
        float newPitch = Utils::Lerp(m_smooth, currentPitch, targetPitch);

        // Actualizar en la JVM
        env->SetFloatField(player, env->GetFieldID(playerClass, Offsets::FIELD_YAW, "F"), newYaw);
        env->SetFloatField(player, env->GetFieldID(playerClass, Offsets::FIELD_PITCH, "F"), newPitch);
    }
}
