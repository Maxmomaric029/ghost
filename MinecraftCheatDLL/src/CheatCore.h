#pragma once
#include <windows.h>
#include <jni.h>
#include <mutex>
#include "OverlayWindow.h"
#include "EntityCache.h"
#include "Aimbot.h"
#include "ESPRenderer.h"

class CheatCore {
public:
    static CheatCore& Instance() { static CheatCore inst; return inst; }

    bool Initialize(JNIEnv* env, HWND hGameWnd);
    void RunFrame();
    void Shutdown();
    void DrawESP(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat);

    bool IsRunning() const { return m_running; }

private:
    CheatCore();
    ~CheatCore();

    OverlayWindow* m_overlay;
    ESPRenderer* m_esp;
    Aimbot* m_aimbot;
    EntityCache* m_cache;
    bool m_running;
    JNIEnv* m_env;
    jobject m_minecraftClient;
    jobject m_localPlayer;
    jobject m_world;
    std::mutex m_mutex;
};
