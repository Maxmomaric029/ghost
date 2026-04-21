#pragma once
#include <windows.h>
#include <jni.h>
#include <mutex>
#include <atomic>
#include "OverlayWindow.h"
#include "EntityCache.h"
#include "Aimbot.h"
#include "ESPRenderer.h"

class CheatCore {
public:
    static CheatCore& Instance() { static CheatCore inst; return inst; }

    bool Initialize(HWND hGameWnd);
    void Run();
    void RunFrame();
    void Shutdown();
    void DrawESP(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat);
    
    // Gestión del Menú
    void HandleInput();
    bool IsMenuVisible() const { return m_menuVisible; }
    
    bool IsRunning() const { return m_running; }
    
    // Estados de módulos para el menú
    bool& GetAimbotEnabled() { return m_aimbotEnabled; }
    bool& GetESPEnabled() { return m_espEnabled; }

private:
    CheatCore();
    ~CheatCore();

    OverlayWindow* m_overlay;
    ESPRenderer* m_esp;
    Aimbot* m_aimbot;
    EntityCache* m_cache;
    
    std::atomic<bool> m_running;
    std::atomic<bool> m_menuVisible;
    bool m_insertWasDown = false;
    
    // Toggles de módulos
    bool m_aimbotEnabled = true;
    bool m_espEnabled = true;

    JNIEnv* m_env;
    jobject m_minecraftClient;
    jobject m_localPlayer;
    jobject m_world;
    std::mutex m_mutex;
};
