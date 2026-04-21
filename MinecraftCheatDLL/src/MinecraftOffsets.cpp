#include "MinecraftOffsets.h"
#include "JVMHelper.h"

namespace Offsets {
    const char* MINECRAFT_CLIENT_CLASS = "net/minecraft/class_310";
    const char* GET_INSTANCE_METHOD = "method_1551";
    const char* PLAYER_FIELD = "field_1724";
    const char* WORLD_FIELD = "field_1687";
    const char* ENTITY_CLASS = "net/minecraft/class_1297";
    const char* POS_X_FIELD = "field_6011"; // Actual position fields for 1.21.4
    const char* POS_Y_FIELD = "field_6012";
    const char* POS_Z_FIELD = "field_6013";
    const char* HEALTH_FIELD = "field_6008"; // health field in LivingEntity
    const char* BOUNDING_BOX_FIELD = "field_5983";
    const char* CLIENT_PLAYER_ENTITY_CLASS = "net/minecraft/class_746";
    const char* YAW_FIELD = "field_6014";
    const char* PITCH_FIELD = "field_5961";
    const char* GAME_RENDERER_CLASS = "net/minecraft/class_757";

    void InitializeOffsets(JNIEnv* env) {
        JVMHelper::FindClassCached(env, MINECRAFT_CLIENT_CLASS);
        JVMHelper::FindClassCached(env, ENTITY_CLASS);
        JVMHelper::FindClassCached(env, CLIENT_PLAYER_ENTITY_CLASS);
        JVMHelper::FindClassCached(env, GAME_RENDERER_CLASS);
        JVMHelper::FindClassCached(env, "net/minecraft/class_638"); // World
        JVMHelper::FindClassCached(env, "net/minecraft/class_238"); // Box
    }
}
