#pragma once
#include <jni.h>
#include <vector>
#include <mutex>
#include "Utils.h"

struct CachedEntity {
    jobject entityRef;
    Vector3 pos;
    Vector3 lastPos;
    Vector3 velocity;
    Vector3 headPos;
    float health;
    bool isEnemy;
    bool isValid;
};

class EntityCache {
public:
    EntityCache();
    ~EntityCache();

    void Update(JNIEnv* env, jobject world, jobject localPlayer);
    std::vector<CachedEntity> GetEntities();

private:
    std::vector<CachedEntity> m_entities;
    std::mutex m_mutex;
};
