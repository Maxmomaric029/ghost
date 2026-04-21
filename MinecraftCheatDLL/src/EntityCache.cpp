#include "EntityCache.h"
#include "JVMHelper.h"

void EntityCache::Update(JNIEnv* env, jobject world, jobject localPlayer) {
    Clear(env);

    jclass worldClass = JVMHelper::FindClassCached(env, "net/minecraft/class_638");
    jmethodID getEntities = JVMHelper::GetMethodIDCached(env, worldClass, "method_18112", "()Ljava/lang/Iterable;");
    jobject iterable = env->CallObjectMethod(world, getEntities);

    jclass iterableClass = env->FindClass("java/lang/Iterable");
    jmethodID iteratorMid = env->GetMethodID(iterableClass, "iterator", "()Ljava/util/Iterator;");
    jobject iterator = env->CallObjectMethod(iterable, iteratorMid);

    jclass iteratorClass = env->FindClass("java/util/Iterator");
    jmethodID hasNextMid = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    jmethodID nextMid = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

    jclass entityClass = JVMHelper::FindClassCached(env, Offsets::ENTITY_CLASS);
    jfieldID xFid = env->GetFieldID(entityClass, Offsets::POS_X_FIELD, "D");
    jfieldID yFid = env->GetFieldID(entityClass, Offsets::POS_Y_FIELD, "D");
    jfieldID zFid = env->GetFieldID(entityClass, Offsets::POS_Z_FIELD, "D");

    while (env->CallBooleanMethod(iterator, hasNextMid)) {
        jobject entity = env->CallObjectMethod(iterator, nextMid);
        if (env->IsSameObject(entity, localPlayer)) {
            env->DeleteLocalRef(entity);
            continue;
        }

        CachedEntity ce;
        ce.position.x = env->GetDoubleField(entity, xFid);
        ce.position.y = env->GetDoubleField(entity, yFid);
        ce.position.z = env->GetDoubleField(entity, zFid);
        ce.headPosition = { ce.position.x, ce.position.y + 1.8, ce.position.z };
        
        // Health and other fields would be read here
        ce.health = 20.0f; 
        ce.isEnemy = true;
        ce.entityRef = env->NewGlobalRef(entity);

        m_entities.push_back(ce);
        env->DeleteLocalRef(entity);
    }

    env->DeleteLocalRef(iterator);
    env->DeleteLocalRef(iterable);
}

void EntityCache::Clear(JNIEnv* env) {
    for (auto& ce : m_entities) {
        env->DeleteGlobalRef(ce.entityRef);
    }
    m_entities.clear();
}
