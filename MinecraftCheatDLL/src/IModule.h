#pragma once
#include <jni.h>

class IModule {
public:
    virtual ~IModule() {}
    virtual void Run(JNIEnv* env, jobject player) = 0;
    virtual const char* GetName() const = 0;
    bool enabled = false;
};
