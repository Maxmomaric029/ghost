#include "MinecraftOffsets.h"
#include "JVMHelper.h"

namespace MinecraftOffsets {
    jfieldID g_YawFieldID = nullptr;
    jfieldID g_PitchFieldID = nullptr;
    jfieldID g_PosXFieldID = nullptr;
    jfieldID g_PosYFieldID = nullptr;
    jfieldID g_PosZFieldID = nullptr;
    jfieldID g_HealthFieldID = nullptr;

    bool Initialize(JNIEnv* env) {
        jclass entityClass = JVMHelper::GetClass(env, CLASS_ENTITY);
        if (!entityClass) return false;

        g_YawFieldID = JVMHelper::GetField(env, entityClass, FIELD_YAW, "F");
        g_PitchFieldID = JVMHelper::GetField(env, entityClass, FIELD_PITCH, "F");
        g_PosXFieldID = JVMHelper::GetField(env, entityClass, FIELD_POS_X, "D");
        g_PosYFieldID = JVMHelper::GetField(env, entityClass, FIELD_POS_Y, "D");
        g_PosZFieldID = JVMHelper::GetField(env, entityClass, FIELD_POS_Z, "D");

        jclass livingClass = JVMHelper::GetClass(env, CLASS_LIVING);
        if (livingClass) {
            g_HealthFieldID = JVMHelper::GetField(env, livingClass, FIELD_HEALTH, "F");
        }

        return g_YawFieldID && g_PitchFieldID && g_PosXFieldID && g_PosYFieldID && g_PosZFieldID;
    }
}
