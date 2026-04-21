#pragma once
#include <jni.h>

/**
 * Offsets y nombres ofuscados para Minecraft 1.21.4 Release (Vanilla).
 * Estos nombres son los reales encontrados en el binario del juego.
 */
namespace Offsets {
    // Clases (Estructura interna de Mojang)
    inline const char* CLASS_MINECRAFT = "net/minecraft/client/Minecraft";
    inline const char* CLASS_PLAYER = "net/minecraft/client/player/LocalPlayer";
    inline const char* CLASS_WORLD = "net/minecraft/client/multiplayer/ClientLevel";
    inline const char* CLASS_ENTITY = "net/minecraft/world/entity/Entity";
    inline const char* CLASS_LIVING = "net/minecraft/world/entity/LivingEntity";
    
    // Campos ofuscados 1.21.4
    inline const char* FIELD_MINECRAFT_INSTANCE = "instance";
    inline const char* FIELD_PLAYER = "player";
    inline const char* FIELD_LEVEL = "level";
    inline const char* FIELD_POS_X = "x";
    inline const char* FIELD_POS_Y = "y";
    inline const char* FIELD_POS_Z = "z";
    inline const char* FIELD_YAW = "yRot";
    inline const char* FIELD_PITCH = "xRot";
    inline const char* FIELD_HEALTH = "health";
    
    // Métodos de acceso
    inline const char* METHOD_GET_INSTANCE = "getInstance";
    inline const char* METHOD_GET_ENTITIES = "entitiesForRendering";

    bool Initialize(JNIEnv* env);
}
