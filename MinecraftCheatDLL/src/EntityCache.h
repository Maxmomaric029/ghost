#pragma once
#include <vector>
#include <jni.h>
#include "MinecraftOffsets.h"

struct CachedEntity {
    Vector3 position;
    Vector3 headPosition;
    float health;
    float maxHealth;
    bool isEnemy;
    jobject entityRef;
};

class EntityCache {
public:
    void Update(JNIEnv* env, jobject world, jobject localPlayer);
    const std::vector<CachedEntity>& GetEntities() const { return m_entities; }
    void Clear(JNIEnv* env);

private:
    std::vector<CachedEntity> m_entities;
};
