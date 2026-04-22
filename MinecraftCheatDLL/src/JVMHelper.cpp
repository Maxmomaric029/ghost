#include "JVMHelper.h"
#include "MinecraftOffsets.h"
#include <iostream>

namespace JVMHelper {
    static JavaVM* g_jvm = nullptr;
    static std::unordered_map<std::string, jclass> g_classCache;
    static std::unordered_map<std::string, jfieldID> g_fieldCache;
    static std::unordered_map<std::string, jmethodID> g_methodCache;
    static std::mutex g_cacheMutex;

    bool Initialize() {
        jsize count = 0;
        jint res = JNI_GetCreatedJavaVMs(&g_jvm, 1, &count);
        printf("[JVM] GetCreatedJavaVMs -> res=%d count=%d\n", res, count);
        
        if (res != JNI_OK || count == 0) {
            printf("[JVM] ERROR: No se pudo localizar la Java VM en el proceso.\n");
            return false;
        }
        
        printf("[JVM] JVM Conectada en: %p\n", g_jvm);
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
        g_methodCache.clear();
    }
    
    JavaVM* GetJVM() {
        return g_jvm;
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

    jclass FindMinecraftClass(JNIEnv* env, const char* name) {
        jclass threadClass = env->FindClass("java/lang/Thread");
        jmethodID currentThread = env->GetStaticMethodID(threadClass, "currentThread", "()Ljava/lang/Thread;");
        jobject thread = env->CallStaticObjectMethod(threadClass, currentThread);
        
        jmethodID getContextCL = env->GetMethodID(threadClass, "getContextClassLoader", "()Ljava/lang/ClassLoader;");
        jobject classLoader = env->CallObjectMethod(thread, getContextCL);
        
        jclass clClass = env->FindClass("java/lang/ClassLoader");
        jmethodID loadClass = env->GetMethodID(clClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        
        std::string dotName(name);
        for (char& c : dotName) if (c == '/') c = '.';
        
        jstring nameStr = env->NewStringUTF(dotName.c_str());
        jclass result = (jclass)env->CallObjectMethod(classLoader, loadClass, nameStr);
        env->DeleteLocalRef(nameStr);
        
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
            printf("[JVM] ClassLoader no encontro: %s\n", name);
            return nullptr;
        }
        
        if (!result) {
            printf("[JVM] loadClass retorno null: %s\n", name);
            return nullptr;
        }
        
        jclass global = (jclass)env->NewGlobalRef(result);
        env->DeleteLocalRef(result);
        return global;
    }

    jclass GetClass(JNIEnv* env, const char* name) {
        std::lock_guard<std::mutex> lock(g_cacheMutex);
        if (g_classCache.count(name)) return g_classCache[name];

        jclass local = env->FindClass(name);
        if (env->ExceptionCheck()) env->ExceptionClear();
        
        if (!local) {
            printf("[JVM] FindClass fallo, intentando con ClassLoader para: %s\n", name);
            local = FindMinecraftClass(env, name);
            if (!local) return nullptr;
            g_classCache[name] = local;
            return local;
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
        std::lock_guard<std::mutex> lock(g_cacheMutex);
        std::string key = std::to_string((uintptr_t)clazz) + name + sig;
        if (g_methodCache.count(key)) return g_methodCache[key];

        jmethodID mid = env->GetMethodID(clazz, name, sig);
        if (!mid) {
            std::string msg = "[JVMHelper] No se encontró el método: " + std::string(name);
            OutputDebugStringA(msg.c_str());
            return nullptr;
        }
        g_methodCache[key] = mid;
        return mid;
    }

    jmethodID GetStaticMethod(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
        std::lock_guard<std::mutex> lock(g_cacheMutex);
        std::string key = "static" + std::to_string((uintptr_t)clazz) + name + sig;
        if (g_methodCache.count(key)) return g_methodCache[key];

        jmethodID mid = env->GetStaticMethodID(clazz, name, sig);
        if (!mid) {
            std::string msg = "[JVMHelper] No se encontró el método estático: " + std::string(name);
            OutputDebugStringA(msg.c_str());
            return nullptr;
        }
        g_methodCache[key] = mid;
        return mid;
    }

    jobject GetMinecraftClient(JNIEnv* env) {
        jclass clientClass = GetClass(env, MinecraftOffsets::CLASS_MINECRAFT);
        if (!clientClass) return nullptr;
        jmethodID getInstance = GetStaticMethod(env, clientClass, MinecraftOffsets::METHOD_GET_INSTANCE, "()L" "net/minecraft/class_310" ";");
        if (!getInstance) return nullptr;
        return env->CallStaticObjectMethod(clientClass, getInstance);
    }

    jobject GetLocalPlayer(JNIEnv* env, jobject client) {
        jclass clientClass = GetClass(env, MinecraftOffsets::CLASS_MINECRAFT);
        jfieldID playerField = GetField(env, clientClass, MinecraftOffsets::FIELD_PLAYER, "L" "net/minecraft/class_746" ";");
        return env->GetObjectField(client, playerField);
    }

    jobject GetWorld(JNIEnv* env, jobject client) {
        jclass clientClass = GetClass(env, MinecraftOffsets::CLASS_MINECRAFT);
        jfieldID worldField = GetField(env, clientClass, MinecraftOffsets::FIELD_LEVEL, "L" "net/minecraft/class_638" ";");
        return env->GetObjectField(client, worldField);
    }

    void ExceptionCheck(JNIEnv* env) {
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
            OutputDebugStringA("[JVMHelper] Excepción de Java capturada y limpiada.");
        }
    }
}
