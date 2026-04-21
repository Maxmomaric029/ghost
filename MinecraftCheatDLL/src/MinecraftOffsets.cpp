#include "MinecraftOffsets.h"
#include "JVMHelper.h"

namespace MinecraftOffsets {
    jfieldID g_YawFieldID = nullptr;
    jfieldID g_PitchFieldID = nullptr;
    jfieldID g_PosXFieldID = nullptr;
    jfieldID g_PosYFieldID = nullptr;
    jfieldID g_PosZFieldID = nullptr;
    jfieldID g_HealthFieldID = nullptr;
    jfieldID g_BoundingBoxFieldID = nullptr;

    bool Initialize(JNIEnv* env) {
        jclass entityClass = JVMHelper::FindClassCached(env, MC_CLASS_ENTITY);
        if (!entityClass) return false;

        g_YawFieldID = JVMHelper::GetFieldIDCached(env, entityClass, MC_FIELD_YAW, "F");
        g_PitchFieldID = JVMHelper::GetFieldIDCached(env, entityClass, MC_FIELD_PITCH, "F");
        g_PosXFieldID = JVMHelper::GetFieldIDCached(env, entityClass, MC_FIELD_POS_X, "D");
        g_PosYFieldID = JVMHelper::GetFieldIDCached(env, entityClass, MC_FIELD_POS_Y, "D");
        g_PosZFieldID = JVMHelper::GetFieldIDCached(env, entityClass, MC_FIELD_POS_Z, "D");
        g_HealthFieldID = JVMHelper::GetFieldIDCached(env, entityClass, MC_FIELD_HEALTH, "F");
        g_BoundingBoxFieldID = JVMHelper::GetFieldIDCached(env, entityClass, MC_FIELD_BOUNDINGBOX, "Lnet/minecraft/class_238;");

        return g_YawFieldID && g_PitchFieldID && g_PosXFieldID && g_PosYFieldID && g_PosZFieldID;
    }

    void Cleanup(JNIEnv* env) {
        // IDs are not references, no cleanup needed unless global refs used for classes
    }
}
