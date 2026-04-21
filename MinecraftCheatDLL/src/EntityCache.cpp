#include "EntityCache.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"

EntityCache::EntityCache() {}
EntityCache::~EntityCache() {}

void EntityCache::Update(JNIEnv* env, jobject world, jobject localPlayer) {
    jclass worldClass = JVMHelper::FindClassCached(env, MC_CLASS_WORLD);
    jmethodID getEntities = JVMHelper::GetMethodIDCached(env, worldClass, MC_METHOD_GETENTITIES, "()Ljava/lang/Iterable;");
    jobject iterable = env->CallObjectMethod(world, getEntities);

    jclass iterableClass = env->FindClass("java/lang/Iterable");
    jmethodID iteratorMid = env->GetMethodID(iterableClass, "iterator", "()Ljava/util/Iterator;");
    jobject iterator = env->CallObjectMethod(iterable, iteratorMid);

    jclass iteratorClass = env->FindClass("java/util/Iterator");
    jmethodID hasNextMid = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    jmethodID nextMid = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

    std::vector<CachedEntity> temp;
    while (env->CallBooleanMethod(iterator, hasNextMid)) {
        jobject entity = env->CallObjectMethod(iterator, nextMid);
        if (env->IsSameObject(entity, localPlayer)) {
            env->DeleteLocalRef(entity);
            continue;
        }

        CachedEntity ce;
        ce.position.x = env->GetDoubleField(entity, MinecraftOffsets::g_PosXFieldID);
        ce.position.y = env->GetDoubleField(entity, MinecraftOffsets::g_PosYFieldID);
        ce.position.z = env->GetDoubleField(entity, MinecraftOffsets::g_PosZFieldID);
        ce.headPos = { ce.position.x, ce.position.y + 1.8, ce.position.z };
        ce.health = env->GetFloatField(entity, MinecraftOffsets::g_HealthFieldID);
        ce.isEnemy = true; // Placeholder for team check logic
        ce.isValid = true;
        ce.entityRef = env->NewGlobalRef(entity);

        temp.push_back(ce);
        env->DeleteLocalRef(entity);
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& old : m_entities) env->DeleteGlobalRef(old.entityRef);
        m_entities = temp;
    }

    env->DeleteLocalRef(iterator);
    env->DeleteLocalRef(iterable);
}

std::vector<CachedEntity> EntityCache::GetEntities() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_entities;
}
