#pragma once
#include <jni.h>

namespace MinecraftOffsets {
    /**
     * NOTA SOBRE OFUSCACIÓN: 
     * Los nombres a continuación son desofuscados (Mojang Mappings). 
     * Funcionarán en entornos de desarrollo (Fabric/Forge dev) o con agentes de remapping.
     * Para Minecraft Vanilla 1.21.4, se deben usar los nombres ofuscados del client.txt.
     * Ejemplo: CLASS_MINECRAFT podría ser "fgo" en vanilla.
     */

    // Clases (Intermediary - Fabric 1.21.4)
    inline const char* CLASS_MINECRAFT = "net/minecraft/class_310";
    inline const char* CLASS_PLAYER = "net/minecraft/class_746";
    inline const char* CLASS_WORLD = "net/minecraft/class_638";
    inline const char* CLASS_ENTITY = "net/minecraft/class_1297";
    inline const char* CLASS_LIVING = "net/minecraft/class_1309";
    inline const char* CLASS_GAMERENDERER = "net/minecraft/class_757";
    
    // Campos (Intermediary)
    inline const char* FIELD_PLAYER = "field_1724";
    inline const char* FIELD_LEVEL = "field_1687";
    inline const char* FIELD_GAMERENDERER = "field_1773";
    inline const char* FIELD_PROJ_MATRIX = "field_21526";
    inline const char* FIELD_MV_MATRIX = "field_21527";
    
    inline const char* FIELD_POS_X = "field_5973";
    inline const char* FIELD_POS_Y = "field_5974";
    inline const char* FIELD_POS_Z = "field_5975";
    inline const char* FIELD_YAW = "field_6031";
    inline const char* FIELD_PITCH = "field_5965";
    inline const char* FIELD_HEALTH = "field_6213";
    
    // Métodos (Intermediary)
    inline const char* METHOD_GET_INSTANCE = "method_1551";
    inline const char* METHOD_GET_ENTITIES = "method_18112";

    // IDs Globales
    extern jfieldID g_YawFieldID;
    extern jfieldID g_PitchFieldID;
    extern jfieldID g_PosXFieldID;
    extern jfieldID g_PosYFieldID;
    extern jfieldID g_PosZFieldID;
    extern jfieldID g_HealthFieldID;
    extern jfieldID g_GameRendererFieldID;
    extern jfieldID g_ProjMatrixFieldID;
    extern jfieldID g_MVMatrixFieldID;
    extern jfieldID g_MatrixFields[16];

    // Method IDs
    extern jmethodID g_HasNextMethodID;
    extern jmethodID g_NextMethodID;
    extern jmethodID g_IteratorMethodID;

    bool Initialize(JNIEnv* env);
}
