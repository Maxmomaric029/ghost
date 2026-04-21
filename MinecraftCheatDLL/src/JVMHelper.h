#pragma once
#include <windows.h>
#include <jni.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

/**
 * ScopedLocalRef: Mecanismo RAII para gestionar referencias locales JNI.
 * Evita el desbordamiento de la tabla de referencias locales (límite de 512).
 */
class ScopedLocalRef {
    JNIEnv* m_env;
    jobject m_ref;
public:
    ScopedLocalRef(JNIEnv* env, jobject ref) : m_env(env), m_ref(ref) {}
    ~ScopedLocalRef() { if (m_ref) m_env->DeleteLocalRef(m_ref); }
    jobject get() const { return m_ref; }
    
    // Operador para usarlo como jobject directamente
    operator jobject() const { return m_ref; }
};

namespace JVMHelper {
    /**
     * Inicializa la conexión con la JVM obteniendo la instancia de JavaVM.
     */
    bool Initialize();

    /**
     * Limpia los recursos globales de JNI.
     */
    void Cleanup();

    /**
     * Obtiene el JNIEnv asociado al hilo actual, adjuntándolo si es necesario.
     */
    JNIEnv* GetEnv();

    /**
     * Busca una clase y guarda una referencia global para optimizar búsquedas.
     */
    jclass GetClass(JNIEnv* env, const char* name);

    /**
     * Helper para obtener FieldIDs con caché.
     */
    jfieldID GetField(JNIEnv* env, jclass clazz, const char* name, const char* sig);

    /**
     * Helper para obtener MethodIDs con caché.
     */
    jmethodID GetMethod(JNIEnv* env, jclass clazz, const char* name, const char* sig);

    /**
     * Verifica y limpia excepciones de Java para evitar crashes.
     */
    void ExceptionCheck(JNIEnv* env);
}
