#pragma once
#include <windows.h>
#include <jni.h>
#include <mutex>
#include <atomic>
#include "OverlayWindow.h"
#include "EntityCache.h"
#include "Aimbot.h"
#include "ESPRenderer.h"
#include "Logger.h"
#include "IModule.h"
#include "Config.h"
#include <vector>

class CheatCore {
public:
    static CheatCore& Instance() { static CheatCore inst; return inst; }

    bool Initialize(HWND hGameWnd);
    void Run();
    void RunFrame();
    void Shutdown();
    void DrawESP(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat);
    
    void HandleInput();
    void ToggleMenu();
    void OnMouseClick(float mx, float my, float winWidth, float winHeight);
    
    EntityCache& GetCache() { return *m_cache; }
    Config& GetConfig() { return m_config; }
    bool IsMenuVisible() const { return m_menuVisible; }
    
    bool IsRunning() const { return m_running; }
    
    bool& GetAimbotEnabled() { return m_aimbotEnabled; }
    bool& GetESPEnabled() { return m_espEnabled; }
    Aimbot* GetAimbot() { return m_aimbot; }

    const Matrix4x4& GetViewMatrix() const { return m_modelViewMatrix; }
    const Matrix4x4& GetProjMatrix() const { return m_projectionMatrix; }
    Vector3 GetLocalPos() const { return m_localPos; }

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
    
    bool m_aimbotEnabled = true;
    bool m_espEnabled = true;

    // Matrices reales de Minecraft
    Matrix4x4 m_modelViewMatrix;
    Matrix4x4 m_projectionMatrix;

    Config m_config;
    std::vector<IModule*> m_modules;
    
    JNIEnv* m_env;
    jobject m_minecraftClient;
    jobject m_localPlayer;
    jobject m_world;
    Vector3 m_localPos;
    std::mutex m_mutex;
};
