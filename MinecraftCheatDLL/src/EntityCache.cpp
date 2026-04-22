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
    std::vector<CachedEntity> oldEntities;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        oldEntities = m_entities;
    }

    jclass worldClass = JVMHelper::GetClass(env, MinecraftOffsets::CLASS_WORLD);
    if (!worldClass) return;
    jmethodID getEntities = JVMHelper::GetMethod(env, worldClass, MinecraftOffsets::METHOD_GET_ENTITIES, "()Ljava/lang/Iterable;");
    if (!getEntities) return;
    
    jobject iterable = env->CallObjectMethod(world, getEntities);
    if (!iterable) return;
    ScopedLocalRef slrIterable(env, iterable);

    jobject iterator = env->CallObjectMethod(iterable, MinecraftOffsets::g_IteratorMethodID);
    if (!iterator) return;
    ScopedLocalRef slrIterator(env, iterator);

    std::vector<CachedEntity> tempEntities;

    while (env->CallBooleanMethod(iterator, MinecraftOffsets::g_HasNextMethodID)) {
        jobject entity = env->CallObjectMethod(iterator, MinecraftOffsets::g_NextMethodID);
        if (!entity) continue;
        
        if (env->IsSameObject(entity, localPlayer)) {
            env->DeleteLocalRef(entity);
            continue;
        }

        ScopedLocalRef slrEntity(env, entity);

        CachedEntity data;
        data.entityRef = env->NewGlobalRef(entity);
        
        data.pos.x = env->GetDoubleField(entity, MinecraftOffsets::g_PosXFieldID);
        data.pos.y = env->GetDoubleField(entity, MinecraftOffsets::g_PosYFieldID);
        data.pos.z = env->GetDoubleField(entity, MinecraftOffsets::g_PosZFieldID);
        data.headPos = { data.pos.x, data.pos.y + 1.62, data.pos.z };
        
        // Calcular velocidad basada en el frame anterior
        data.velocity = { 0, 0, 0 };
        data.lastPos = data.pos;
        for (const auto& old : oldEntities) {
            if (env->IsSameObject(entity, old.entityRef)) {
                data.velocity = { 
                    (float)(data.pos.x - old.pos.x),
                    (float)(data.pos.y - old.pos.y),
                    (float)(data.pos.z - old.pos.z)
                };
                data.lastPos = old.pos;
                break;
            }
        }

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
        // Limpiar referencias antiguas
        for (auto& old : m_entities) {
            if (old.entityRef) env->DeleteGlobalRef(old.entityRef);
        }
        m_entities = tempEntities;
    }
}

std::vector<CachedEntity> EntityCache::GetEntities() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_entities;
}
