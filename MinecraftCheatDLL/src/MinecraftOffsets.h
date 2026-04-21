#pragma once
#include <jni.h>

namespace MinecraftOffsets {
    // Clases
    inline const char* CLASS_MINECRAFT = "net/minecraft/client/Minecraft";
    inline const char* CLASS_PLAYER = "net/minecraft/client/player/LocalPlayer";
    inline const char* CLASS_WORLD = "net/minecraft/client/multiplayer/ClientLevel";
    inline const char* CLASS_ENTITY = "net/minecraft/world/entity/Entity";
    inline const char* CLASS_LIVING = "net/minecraft/world/entity/LivingEntity";
    inline const char* CLASS_GAMERENDERER = "net/minecraft/client/renderer/GameRenderer";
    
    // Campos
    inline const char* FIELD_MINECRAFT_INSTANCE = "instance";
    inline const char* FIELD_PLAYER = "player";
    inline const char* FIELD_LEVEL = "level";
    inline const char* FIELD_GAMERENDERER = "gameRenderer";
    inline const char* FIELD_PROJ_MATRIX = "projectionMatrix";
    inline const char* FIELD_MV_MATRIX = "modelViewMatrix";
    
    inline const char* FIELD_POS_X = "x";
    inline const char* FIELD_POS_Y = "y";
    inline const char* FIELD_POS_Z = "z";
    inline const char* FIELD_YAW = "yRot";
    inline const char* FIELD_PITCH = "xRot";
    inline const char* FIELD_HEALTH = "health";
    
    // Métodos
    inline const char* METHOD_GET_INSTANCE = "getInstance";
    inline const char* METHOD_GET_ENTITIES = "entitiesForRendering";

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

    bool Initialize(JNIEnv* env);
}
