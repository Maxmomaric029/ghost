#pragma once
#include <windows.h>
#include <jni.h>
#include <string>
#include <unordered_map>
#include <mutex>

class JVMHelper {
public:
    static bool Initialize();
    static void Cleanup();

    static JavaVM* GetJavaVM();
    static JNIEnv* AttachThreadToJVM();
    static void DetachThreadFromJVM();

    static jclass FindClassCached(JNIEnv* env, const char* name);
    static jfieldID GetFieldIDCached(JNIEnv* env, jclass clazz, const char* name, const char* sig);
    static jmethodID GetMethodIDCached(JNIEnv* env, jclass clazz, const char* name, const char* sig);
    static jmethodID GetStaticMethodIDCached(JNIEnv* env, jclass clazz, const char* name, const char* sig);

    static jobject GetMinecraftClient(JNIEnv* env);
    static jobject GetLocalPlayer(JNIEnv* env, jobject client);
    static jobject GetWorld(JNIEnv* env, jobject client);

    static bool CheckException(JNIEnv* env);

private:
    static JavaVM* m_vm;
    static std::unordered_map<std::string, jclass> m_classCache;
    static std::unordered_map<std::string, jfieldID> m_fieldCache;
    static std::unordered_map<std::string, jmethodID> m_methodCache;
    static std::mutex m_mutex;
};
