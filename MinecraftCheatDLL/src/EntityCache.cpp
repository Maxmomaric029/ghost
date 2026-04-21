#include "EntityCache.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"

EntityCache::EntityCache() {}

/**
 * Destructor que asegura la liberación de todas las referencias globales JNI.
 * Previene crashes por agotamiento de la tabla de referencias de la JVM.
 */
EntityCache::~EntityCache() {
    JNIEnv* env = JVMHelper::GetEnv();
    if (!env) return;

    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& entity : m_entities) {
        if (entity.entityRef) env->DeleteGlobalRef(entity.entityRef);
    }
}

/**
 * Actualiza la caché local leyendo los datos de la JVM.
 * Implementa limpieza de referencias y RAII local.
 */
void EntityCache::Update(JNIEnv* env, jobject world) {
    // 1. Limpiar caché previa y sus referencias globales
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& entity : m_entities) {
            if (entity.entityRef) env->DeleteGlobalRef(entity.entityRef);
        }
        m_entities.clear();
    }

    jclass worldClass = JVMHelper::GetClass(env, Offsets::CLASS_WORLD);
    jmethodID getEntities = env->GetMethodID(worldClass, Offsets::METHOD_GET_ENTITIES, "()Ljava/lang/Iterable;");
    
    jobject iterable = env->CallObjectMethod(world, getEntities);
    if (!iterable) return;
    ScopedLocalRef slrIterable(env, iterable);

    // Iteración sobre la lista de entidades
    jclass iterClass = env->FindClass("java/util/Iterator");
    jmethodID hasNext = env->GetMethodID(iterClass, "hasNext", "()Z");
    jmethodID next = env->GetMethodID(iterClass, "next", "()Ljava/lang/Object;");

    jobject iterator = env->CallObjectMethod(iterable, env->GetMethodID(env->FindClass("java/lang/Iterable"), "iterator", "()Ljava/util/Iterator;"));
    ScopedLocalRef slrIterator(env, iterator);

    std::vector<CachedData> tempEntities;

    while (env->CallBooleanMethod(iterator, hasNext)) {
        jobject entity = env->CallObjectMethod(iterator, next);
        if (!entity) continue;
        
        // Uso de RAII para referencias locales dentro del bucle
        ScopedLocalRef slrEntity(env, entity);

        CachedData data;
        data.entityRef = env->NewGlobalRef(entity); // Guardar para uso futuro
        
        jclass entClass = env->GetObjectClass(entity);
        ScopedLocalRef slrEntClass(env, entClass);

        // Obtener posición
        data.pos.x = env->GetDoubleField(entity, env->GetFieldID(entClass, Offsets::FIELD_POS_X, "D"));
        data.pos.y = env->GetDoubleField(entity, env->GetFieldID(entClass, Offsets::FIELD_POS_Y, "D"));
        data.pos.z = env->GetDoubleField(entity, env->GetFieldID(entClass, Offsets::FIELD_POS_Z, "D"));
        
        // Obtener salud
        jclass livingClass = JVMHelper::GetClass(env, Offsets::CLASS_LIVING);
        if (env->IsInstanceOf(entity, livingClass)) {
            data.health = env->GetFloatField(entity, env->GetFieldID(livingClass, Offsets::FIELD_HEALTH, "F"));
        } else {
            data.health = 0.0f;
        }

        tempEntities.push_back(data);
    }

    // Actualización atómica de la caché
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_entities = tempEntities;
    }
}

std::vector<CachedData> EntityCache::GetEntities() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_entities;
}
