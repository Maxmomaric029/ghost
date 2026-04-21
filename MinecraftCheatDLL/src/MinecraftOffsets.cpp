#include "MinecraftOffsets.h"
#include "JVMHelper.h"

namespace MinecraftOffsets {
    jfieldID g_YawFieldID = nullptr;
    jfieldID g_PitchFieldID = nullptr;
    jfieldID g_PosXFieldID = nullptr;
    jfieldID g_PosYFieldID = nullptr;
    jfieldID g_PosZFieldID = nullptr;
    jfieldID g_HealthFieldID = nullptr;
    jfieldID g_MVMatrixFieldID = nullptr;
    jfieldID g_MatrixFields[16] = { nullptr };

    jmethodID g_HasNextMethodID = nullptr;
    jmethodID g_NextMethodID = nullptr;
    jmethodID g_IteratorMethodID = nullptr;

    bool Initialize(JNIEnv* env) {
        jclass clientClass = JVMHelper::GetClass(env, CLASS_MINECRAFT);
        if (!clientClass) return false;
        g_GameRendererFieldID = JVMHelper::GetField(env, clientClass, FIELD_GAMERENDERER, "L" "net/minecraft/class_757" ";");

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

        jclass matClass = JVMHelper::GetClass(env, "org/joml/Matrix4f");
        if (matClass) {
            const char* fields[] = { "m00","m01","m02","m03","m10","m11","m12","m13","m20","m21","m22","m23","m30","m31","m32","m33" };
            for (int i = 0; i < 16; i++) {
                g_MatrixFields[i] = JVMHelper::GetField(env, matClass, fields[i], "F");
            }
        }

        // Inicializar métodos de Iterator e Iterable una sola vez
        jclass iterClass = JVMHelper::GetClass(env, "java/util/Iterator");
        if (iterClass) {
            g_HasNextMethodID = JVMHelper::GetMethod(env, iterClass, "hasNext", "()Z");
            g_NextMethodID = JVMHelper::GetMethod(env, iterClass, "next", "()Ljava/lang/Object;");
        }

        jclass iterableClass = JVMHelper::GetClass(env, "java/lang/Iterable");
        if (iterableClass) {
            g_IteratorMethodID = JVMHelper::GetMethod(env, iterableClass, "iterator", "()Ljava/util/Iterator;");
        }

        return g_YawFieldID && g_PitchFieldID && g_PosXFieldID && g_PosYFieldID && g_PosZFieldID;
    }
}
