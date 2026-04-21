#include "CheatCore.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"

CheatCore::CheatCore() : m_overlay(nullptr), m_esp(nullptr), m_aimbot(nullptr), m_cache(nullptr), m_running(false), m_env(nullptr), m_minecraftClient(nullptr), m_localPlayer(nullptr), m_world(nullptr) {}

CheatCore::~CheatCore() { Shutdown(); }

bool CheatCore::Initialize(HWND hGameWnd) {
    m_env = JVMHelper::GetEnv();
    if (!m_env) return false;

    if (!MinecraftOffsets::Initialize(m_env)) {
        OutputDebugStringA("[CheatCore] Fallo al inicializar offsets.");
        return false;
    }

    m_minecraftClient = m_env->NewGlobalRef(JVMHelper::GetMinecraftClient(m_env));
    if (!m_minecraftClient) return false;

    m_localPlayer = m_env->NewGlobalRef(JVMHelper::GetLocalPlayer(m_env, m_minecraftClient));
    m_world = m_env->NewGlobalRef(JVMHelper::GetWorld(m_env, m_minecraftClient));

    m_cache = new EntityCache();
    m_aimbot = new Aimbot();
    m_esp = new ESPRenderer();
    m_overlay = new OverlayWindow();

    if (!m_overlay->Create(hGameWnd)) return false;

    m_running = true;
    return true;
}

void CheatCore::Run() {
    RunFrame();
}

void CheatCore::RunFrame() {
    if (!m_running) return;
    JNIEnv* env = JVMHelper::GetEnv();
    if (!env) return;

    m_cache->Update(env, m_world, m_localPlayer);
    m_aimbot->Run(*m_cache, env, m_localPlayer);

    m_overlay->Invalidate();
}

void CheatCore::DrawESP(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_esp && m_cache) {
        ESPRenderer::Draw(rt, brush, textFormat, *m_cache);
    }
}

void CheatCore::Shutdown() {
    m_running = false;
    if (m_overlay) { m_overlay->Destroy(); delete m_overlay; m_overlay = nullptr; }
    
    JNIEnv* env = JVMHelper::GetEnv();
    if (env) {
        if (m_minecraftClient) env->DeleteGlobalRef(m_minecraftClient);
        if (m_localPlayer) env->DeleteGlobalRef(m_localPlayer);
        if (m_world) env->DeleteGlobalRef(m_world);
    }

    if (m_cache) delete m_cache;
    if (m_aimbot) delete m_aimbot;
    if (m_esp) delete m_esp;
}
