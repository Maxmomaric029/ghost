#include "JVMHelper.h"
#include "MinecraftOffsets.h"
#include <iostream>

JavaVM* JVMHelper::m_vm = nullptr;
std::unordered_map<std::string, jclass> JVMHelper::m_classCache;
std::unordered_map<std::string, jfieldID> JVMHelper::m_fieldCache;
std::unordered_map<std::string, jmethodID> JVMHelper::m_methodCache;
std::mutex JVMHelper::m_mutex;

bool JVMHelper::Initialize() {
    std::lock_guard<std::mutex> lock(m_mutex);
    jsize count = 0;
    if (JNI_GetCreatedJavaVMs(&m_vm, 1, &count) != JNI_OK || count == 0) {
        OutputDebugStringA("[JVMHelper] No se pudo obtener la Java VM.");
        return false;
    }
    return true;
}

void JVMHelper::Cleanup() {
    std::lock_guard<std::mutex> lock(m_mutex);
    JNIEnv* env = nullptr;
    if (m_vm->GetEnv((void**)&env, JNI_VERSION_1_8) == JNI_OK) {
        for (auto& pair : m_classCache) {
            env->DeleteGlobalRef(pair.second);
        }
    }
    m_classCache.clear();
    m_fieldCache.clear();
    m_methodCache.clear();
}

JavaVM* JVMHelper::GetJavaVM() { return m_vm; }

JNIEnv* JVMHelper::AttachThreadToJVM() {
    JNIEnv* env = nullptr;
    jint res = m_vm->GetEnv((void**)&env, JNI_VERSION_1_8);
    if (res == JNI_EDETACHED) {
        JavaVMAttachArgs args = { JNI_VERSION_1_8, (char*)"GhostClientThread", NULL };
        if (m_vm->AttachCurrentThread((void**)&env, &args) != JNI_OK) {
            OutputDebugStringA("[JVMHelper] Fallo al adjuntar hilo a la JVM.");
            return nullptr;
        }
    }
    return env;
}

void JVMHelper::DetachThreadFromJVM() {
    m_vm->DetachCurrentThread();
}

jclass JVMHelper::FindClassCached(JNIEnv* env, const char* name) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_classCache.count(name)) return m_classCache[name];
    jclass local = env->FindClass(name);
    if (!local) {
        std::string err = "[JVMHelper] No se encontró la clase: " + std::string(name);
        OutputDebugStringA(err.c_str());
        return nullptr;
    }
    jclass global = (jclass)env->NewGlobalRef(local);
    env->DeleteLocalRef(local);
    m_classCache[name] = global;
    return global;
}

jfieldID JVMHelper::GetFieldIDCached(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string key = std::to_string((uintptr_t)clazz) + name + sig;
    if (m_fieldCache.count(key)) return m_fieldCache[key];
    jfieldID fid = env->GetFieldID(clazz, name, sig);
    if (fid) m_fieldCache[key] = fid;
    else OutputDebugStringA(("[JVMHelper] Fallo al obtener FieldID: " + std::string(name)).c_str());
    return fid;
}

jmethodID JVMHelper::GetMethodIDCached(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string key = std::to_string((uintptr_t)clazz) + name + sig;
    if (m_methodCache.count(key)) return m_methodCache[key];
    jmethodID mid = env->GetMethodID(clazz, name, sig);
    if (mid) m_methodCache[key] = mid;
    else OutputDebugStringA(("[JVMHelper] Fallo al obtener MethodID: " + std::string(name)).c_str());
    return mid;
}

jmethodID JVMHelper::GetStaticMethodIDCached(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string key = "static" + std::to_string((uintptr_t)clazz) + name + sig;
    if (m_methodCache.count(key)) return m_methodCache[key];
    jmethodID mid = env->GetStaticMethodID(clazz, name, sig);
    if (mid) m_methodCache[key] = mid;
    else OutputDebugStringA(("[JVMHelper] Fallo al obtener Static MethodID: " + std::string(name)).c_str());
    return mid;
}

jobject JVMHelper::GetMinecraftClient(JNIEnv* env) {
    jclass clientClass = FindClassCached(env, MC_CLASS_MINECRAFT);
    if (!clientClass) return nullptr;
    jmethodID getInstance = GetStaticMethodIDCached(env, clientClass, MC_METHOD_GETINSTANCE, "()L" MC_CLASS_MINECRAFT ";");
    if (!getInstance) return nullptr;
    return env->CallStaticObjectMethod(clientClass, getInstance);
}

jobject JVMHelper::GetLocalPlayer(JNIEnv* env, jobject client) {
    jclass clientClass = FindClassCached(env, MC_CLASS_MINECRAFT);
    jfieldID playerField = GetFieldIDCached(env, clientClass, MC_FIELD_PLAYER, "L" MC_CLASS_CLIENTPLAYERENTITY ";");
    return env->GetObjectField(client, playerField);
}

jobject JVMHelper::GetWorld(JNIEnv* env, jobject client) {
    jclass clientClass = FindClassCached(env, MC_CLASS_MINECRAFT);
    jfieldID worldField = GetFieldIDCached(env, clientClass, MC_FIELD_WORLD, "L" MC_CLASS_WORLD ";");
    return env->GetObjectField(client, worldField);
}

bool JVMHelper::CheckException(JNIEnv* env) {
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        OutputDebugStringA("[JVMHelper] Excepción JNI detectada y limpiada.");
        return true;
    }
    return false;
}
