#include "JVMHelper.h"
#include <iostream>

namespace JVMHelper {
    static JavaVM* g_jvm = nullptr;
    static std::unordered_map<std::string, jclass> g_classCache;
    static std::unordered_map<std::string, jfieldID> g_fieldCache;
    static std::mutex g_cacheMutex;

    bool Initialize() {
        jsize count = 0;
        if (JNI_GetCreatedJavaVMs(&g_jvm, 1, &count) != JNI_OK || count == 0) {
            OutputDebugStringA("[JVMHelper] No se pudo localizar la Java VM en el proceso.");
            return false;
        }
        return true;
    }

    void Cleanup() {
        JNIEnv* env = GetEnv();
        if (!env) return;

        std::lock_guard<std::mutex> lock(g_cacheMutex);
        for (auto& pair : g_classCache) {
            env->DeleteGlobalRef(pair.second);
        }
        g_classCache.clear();
        g_fieldCache.clear();
    }

    JNIEnv* GetEnv() {
        JNIEnv* env = nullptr;
        jint res = g_jvm->GetEnv((void**)&env, JNI_VERSION_1_8);
        if (res == JNI_EDETACHED) {
            JavaVMAttachArgs args = { JNI_VERSION_1_8, (char*)"GhostClientThread", NULL };
            if (g_jvm->AttachCurrentThread((void**)&env, &args) != JNI_OK) {
                OutputDebugStringA("[JVMHelper] Error fatal: No se pudo adjuntar el hilo a la JVM.");
                return nullptr;
            }
        }
        return env;
    }

    jclass GetClass(JNIEnv* env, const char* name) {
        std::lock_guard<std::mutex> lock(g_cacheMutex);
        if (g_classCache.count(name)) return g_classCache[name];

        jclass local = env->FindClass(name);
        if (!local) {
            std::string msg = "[JVMHelper] No se encontró la clase: " + std::string(name);
            OutputDebugStringA(msg.c_str());
            return nullptr;
        }

        jclass global = (jclass)env->NewGlobalRef(local);
        env->DeleteLocalRef(local);
        g_classCache[name] = global;
        return global;
    }

    jfieldID GetField(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
        std::lock_guard<std::mutex> lock(g_cacheMutex);
        std::string key = std::to_string((uintptr_t)clazz) + name + sig;
        if (g_fieldCache.count(key)) return g_fieldCache[key];

        jfieldID fid = env->GetFieldID(clazz, name, sig);
        if (!fid) {
            std::string msg = "[JVMHelper] No se encontró el campo: " + std::string(name);
            OutputDebugStringA(msg.c_str());
            return nullptr;
        }
        g_fieldCache[key] = fid;
        return fid;
    }

    jmethodID GetMethod(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
        // Implementación similar con caché para métodos
        return env->GetMethodID(clazz, name, sig);
    }

    void ExceptionCheck(JNIEnv* env) {
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
            OutputDebugStringA("[JVMHelper] Excepción de Java capturada y limpiada.");
        }
    }
}
