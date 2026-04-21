#include "MinecraftOffsets.h"
#include "JVMHelper.h"

namespace MinecraftOffsets {
    jfieldID g_YawFieldID = nullptr;
    jfieldID g_PitchFieldID = nullptr;
    jfieldID g_PosXFieldID = nullptr;
    jfieldID g_PosYFieldID = nullptr;
    jfieldID g_PosZFieldID = nullptr;
    jfieldID g_HealthFieldID = nullptr;
    jfieldID g_GameRendererFieldID = nullptr;
    jfieldID g_ProjMatrixFieldID = nullptr;
    jfieldID g_MVMatrixFieldID = nullptr;

    bool Initialize(JNIEnv* env) {
        jclass clientClass = JVMHelper::GetClass(env, CLASS_MINECRAFT);
        if (!clientClass) return false;
        g_GameRendererFieldID = JVMHelper::GetField(env, clientClass, FIELD_GAMERENDERER, "L" "net/minecraft/client/renderer/GameRenderer" ";");

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

        jclass grClass = JVMHelper::GetClass(env, CLASS_GAMERENDERER);
        if (grClass) {
            g_ProjMatrixFieldID = JVMHelper::GetField(env, grClass, FIELD_PROJ_MATRIX, "L" "org/joml/Matrix4f" ";");
            g_MVMatrixFieldID = JVMHelper::GetField(env, grClass, FIELD_MV_MATRIX, "L" "org/joml/Matrix4f" ";");
        }

        return g_YawFieldID && g_PitchFieldID && g_PosXFieldID && g_PosYFieldID && g_PosZFieldID;
    }
}
