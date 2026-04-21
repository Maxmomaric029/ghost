#include "CheatCore.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"

CheatCore::CheatCore() : m_overlay(nullptr), m_esp(nullptr), m_aimbot(nullptr), m_cache(nullptr), m_running(false), m_menuVisible(false), m_env(nullptr), m_minecraftClient(nullptr), m_localPlayer(nullptr), m_world(nullptr) {}

CheatCore::~CheatCore() { Shutdown(); }

bool CheatCore::Initialize(HWND hGameWnd) {
    m_env = JVMHelper::GetEnv();
    if (!m_env) return false;

    if (!MinecraftOffsets::Initialize(m_env)) {
        OutputDebugStringA("[CheatCore] Fallo al inicializar offsets.");
        return false;
    }

    jobject client = JVMHelper::GetMinecraftClient(m_env);
    if (!client) return false;
    m_minecraftClient = m_env->NewGlobalRef(client);
    m_env->DeleteLocalRef(client);

    jobject localPlayer = JVMHelper::GetLocalPlayer(m_env, m_minecraftClient);
    if (localPlayer) {
        m_localPlayer = m_env->NewGlobalRef(localPlayer);
        m_env->DeleteLocalRef(localPlayer);
    }

    jobject world = JVMHelper::GetWorld(m_env, m_minecraftClient);
    if (world) {
        m_world = m_env->NewGlobalRef(world);
        m_env->DeleteLocalRef(world);
    }

    m_cache = new EntityCache();
    m_aimbot = new Aimbot();
    m_esp = new ESPRenderer();
    m_overlay = new OverlayWindow();

    if (!m_overlay->Create(hGameWnd)) return false;

    m_running = true;
    return true;
}

void ReadJOMLMatrix(JNIEnv* env, jobject matrixObj, Matrix4x4& out) {
    if (!matrixObj) return;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            jfieldID fid = MinecraftOffsets::g_MatrixFields[i * 4 + j];
            if (fid) out.m[i][j] = env->GetFloatField(matrixObj, fid);
        }
    }
}

void CheatCore::HandleInput() {
    bool insertIsDown = GetAsyncKeyState(VK_INSERT) & 0x8000;
    if (insertIsDown && !m_insertWasDown) {
        m_menuVisible = !m_menuVisible;
        if (m_overlay) m_overlay->SetClickThrough(!m_menuVisible);
    }
    m_insertWasDown = insertIsDown;
}

void CheatCore::OnMouseClick(float mx, float my) {
    if (!m_menuVisible) return;
    
    // El menú es de 350x400 y está centrado.
    // Lógica de detección simplificada para los botones visuales del menú
    if (mx > 100 && mx < 400 && my > 150 && my < 180) m_aimbotEnabled = !m_aimbotEnabled;
    if (mx > 100 && mx < 400 && my > 200 && my < 230) m_espEnabled = !m_espEnabled;
}

void CheatCore::Run() {
    RunFrame();
}

void CheatCore::RunFrame() {
    if (!m_running) return;
    HandleInput();

    JNIEnv* env = JVMHelper::GetEnv();
    if (!env) return;

    // Actualizar dinámicamente el jugador y el mundo para evitar stale references
    jobject currentPlayer = JVMHelper::GetLocalPlayer(env, m_minecraftClient);
    if (!env->IsSameObject(currentPlayer, m_localPlayer)) {
        if (m_localPlayer) env->DeleteGlobalRef(m_localPlayer);
        m_localPlayer = currentPlayer ? env->NewGlobalRef(currentPlayer) : nullptr;
    }
    if (currentPlayer) env->DeleteLocalRef(currentPlayer);

    jobject currentWorld = JVMHelper::GetWorld(env, m_minecraftClient);
    if (!env->IsSameObject(currentWorld, m_world)) {
        if (m_world) env->DeleteGlobalRef(m_world);
        m_world = currentWorld ? env->NewGlobalRef(currentWorld) : nullptr;
    }
    if (currentWorld) env->DeleteLocalRef(currentWorld);

    if (!m_localPlayer || !m_world) return;

    jobject gameRenderer = env->GetObjectField(m_minecraftClient, MinecraftOffsets::g_GameRendererFieldID);
    if (gameRenderer) {
        jobject projMatObj = env->GetObjectField(gameRenderer, MinecraftOffsets::g_ProjMatrixFieldID);
        jobject mvMatObj = env->GetObjectField(gameRenderer, MinecraftOffsets::g_MVMatrixFieldID);
        
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            ReadJOMLMatrix(env, projMatObj, m_projectionMatrix);
            ReadJOMLMatrix(env, mvMatObj, m_modelViewMatrix);
        }
        
        env->DeleteLocalRef(projMatObj);
        env->DeleteLocalRef(mvMatObj);
        env->DeleteLocalRef(gameRenderer);
    }

    m_cache->Update(env, m_world, m_localPlayer);
    if (m_aimbotEnabled) m_aimbot->Run(*m_cache, env, m_localPlayer);

    m_overlay->Invalidate();
}

void CheatCore::DrawESP(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_espEnabled && m_esp && m_cache) {
        // CORRECCIÓN: Eliminada la llamada a UpdateMatrices redundante
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
