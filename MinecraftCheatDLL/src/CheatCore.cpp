#include "CheatCore.h"
#include "JVMHelper.h"
#include "MinecraftOffsets.h"

bool CheatCore::Initialize() {
    if (!JVMHelper::Initialize()) return false;
    JNIEnv* env = JVMHelper::AttachThreadToJVM();
    if (!env) return false;

    Offsets::InitializeOffsets(env);

    m_mcWnd = FindWindowW(L"GLFW30", L"Minecraft 1.21.4");
    if (!m_mcWnd) return false;

    if (!m_overlay.Create(m_mcWnd)) return false;

    m_active = true;
    return true;
}

void CheatCore::RunFrame() {
    if (!m_active) return;
    JNIEnv* env = JVMHelper::AttachThreadToJVM();
    if (!env) return;

    jclass clientClass = JVMHelper::FindClassCached(env, Offsets::MINECRAFT_CLIENT_CLASS);
    jmethodID getInstance = JVMHelper::GetStaticMethodIDCached(env, clientClass, Offsets::GET_INSTANCE_METHOD, "()Lnet/minecraft/class_310;");
    jobject mc = env->CallStaticObjectMethod(clientClass, getInstance);
    jobject player = env->GetObjectField(mc, JVMHelper::GetFieldIDCached(env, clientClass, Offsets::PLAYER_FIELD, "Lnet/minecraft/class_746;"));
    jobject world = env->GetObjectField(mc, JVMHelper::GetFieldIDCached(env, clientClass, Offsets::WORLD_FIELD, "Lnet/minecraft/class_638;"));

    if (player && world) {
        m_cache.Update(env, world, player);
        m_aimbot.Run(m_cache, env, player);
        
        m_overlay.Invalidate(); 
        // Logic for triggering Draw inside OverlayWindow::Render
        ID2D1HwndRenderTarget* rt = m_overlay.GetRT();
        if (rt) {
            rt->BeginDraw();
            rt->Clear(D2D1::ColorF(0,0,0,0));
            m_esp.Draw(rt, m_overlay.GetBrush(), m_overlay.GetTextFormat(), m_cache);
            rt->EndDraw();
        }
    }

    JVMHelper::CheckException(env);
}

void CheatCore::Shutdown() {
    m_active = false;
    m_overlay.Destroy();
    JVMHelper::Cleanup();
}
