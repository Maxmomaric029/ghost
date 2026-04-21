#pragma once
#include <windows.h>
#include "OverlayWindow.h"
#include "ESPRenderer.h"
#include "Aimbot.h"
#include "EntityCache.h"

class CheatCore {
public:
    static CheatCore& Instance() { static CheatCore inst; return inst; }

    bool Initialize();
    void RunFrame();
    void Shutdown();

    bool IsActive() const { return m_active; }

private:
    CheatCore() : m_active(false) {}
    ~CheatCore() = default;

    OverlayWindow m_overlay;
    ESPRenderer m_esp;
    Aimbot m_aimbot;
    EntityCache m_cache;
    bool m_active;
    HWND m_mcWnd;
};
