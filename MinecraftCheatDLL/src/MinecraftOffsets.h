#pragma once
#include <jni.h>

#define MC_CLASS_MINECRAFT "net/minecraft/class_310"
#define MC_CLASS_CLIENTPLAYERENTITY "net/minecraft/class_746"
#define MC_CLASS_ENTITY "net/minecraft/class_1297"
#define MC_CLASS_WORLD "net/minecraft/class_638"
#define MC_CLASS_GAMERENDERER "net/minecraft/class_757"
#define MC_CLASS_MATRIX4F "net/minecraft/class_1158"

#define MC_FIELD_PLAYER "field_1724"
#define MC_FIELD_WORLD "field_1687"
#define MC_FIELD_YAW "field_6014"
#define MC_FIELD_PITCH "field_5961"
#define MC_FIELD_HEALTH "field_6008"
#define MC_FIELD_POS_X "field_6011"
#define MC_FIELD_POS_Y "field_6012"
#define MC_FIELD_POS_Z "field_6013"
#define MC_FIELD_BOUNDINGBOX "field_5983"

#define MC_METHOD_GETINSTANCE "method_1551"
#define MC_METHOD_GETENTITIES "method_18112"
#define MC_METHOD_GETPROJECTIONMATRIX "method_31931"
#define MC_METHOD_GETMODELVIEWMATRIX "method_31932"

namespace MinecraftOffsets {
    extern jfieldID g_YawFieldID;
    extern jfieldID g_PitchFieldID;
    extern jfieldID g_PosXFieldID;
    extern jfieldID g_PosYFieldID;
    extern jfieldID g_PosZFieldID;
    extern jfieldID g_HealthFieldID;
    extern jfieldID g_BoundingBoxFieldID;

    bool Initialize(JNIEnv* env);
    void Cleanup(JNIEnv* env);
}
