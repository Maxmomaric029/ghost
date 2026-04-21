#include "EntityCache.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"

EntityCache::EntityCache() {}

EntityCache::~EntityCache() {
    JNIEnv* env = JVMHelper::GetEnv();
    if (!env) return;

    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& entity : m_entities) {
        if (entity.entityRef) env->DeleteGlobalRef(entity.entityRef);
    }
}

void EntityCache::Update(JNIEnv* env, jobject world, jobject localPlayer) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& entity : m_entities) {
            if (entity.entityRef) env->DeleteGlobalRef(entity.entityRef);
        }
        m_entities.clear();
    }

    jclass worldClass = JVMHelper::GetClass(env, Offsets::CLASS_WORLD);
    if (!worldClass) return;
    jmethodID getEntities = env->GetMethodID(worldClass, Offsets::METHOD_GET_ENTITIES, "()Ljava/lang/Iterable;");
    if (!getEntities) return;
    
    jobject iterable = env->CallObjectMethod(world, getEntities);
    if (!iterable) return;
    ScopedLocalRef slrIterable(env, iterable);

    jclass iterClass = env->FindClass("java/util/Iterator");
    jmethodID hasNext = env->GetMethodID(iterClass, "hasNext", "()Z");
    jmethodID next = env->GetMethodID(iterClass, "next", "()Ljava/lang/Object;");

    jclass iterableInterface = env->FindClass("java/lang/Iterable");
    jobject iterator = env->CallObjectMethod(iterable, env->GetMethodID(iterableInterface, "iterator", "()Ljava/util/Iterator;"));
    ScopedLocalRef slrIterator(env, iterator);

    std::vector<CachedEntity> tempEntities;

    while (env->CallBooleanMethod(iterator, hasNext)) {
        jobject entity = env->CallObjectMethod(iterator, next);
        if (!entity) continue;
        
        if (env->IsSameObject(entity, localPlayer)) {
            env->DeleteLocalRef(entity);
            continue;
        }

        ScopedLocalRef slrEntity(env, entity);

        CachedEntity data;
        data.entityRef = env->NewGlobalRef(entity);
        
        jclass entClass = env->GetObjectClass(entity);
        ScopedLocalRef slrEntClass(env, entClass);

        data.pos.x = env->GetDoubleField(entity, MinecraftOffsets::g_PosXFieldID);
        data.pos.y = env->GetDoubleField(entity, MinecraftOffsets::g_PosYFieldID);
        data.pos.z = env->GetDoubleField(entity, MinecraftOffsets::g_PosZFieldID);
        data.headPos = { data.pos.x, data.pos.y + 1.62, data.pos.z };
        
        if (MinecraftOffsets::g_HealthFieldID) {
            data.health = env->GetFloatField(entity, MinecraftOffsets::g_HealthFieldID);
        } else {
            data.health = 20.0f;
        }

        data.isEnemy = true;
        data.isValid = true;

        tempEntities.push_back(data);
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_entities = tempEntities;
    }
}

std::vector<CachedEntity> EntityCache::GetEntities() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_entities;
}
