#pragma once
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <thread>
#include <atomic>

class OverlayWindow {
public:
    OverlayWindow();
    ~OverlayWindow();

    bool Create(HWND hTarget);
    void Destroy();
    void Invalidate();
    void SetClickThrough(bool enabled);
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    static HHOOK s_keyboardHook;
    void Run();
    void UpdatePosition();
    void OnPaint();
    void DrawMenu(ID2D1RenderTarget* rt);
    void DrawToggle(ID2D1RenderTarget* rt, float x, float y, const wchar_t* label, bool enabled);
    void DrawSlider(ID2D1RenderTarget* rt, float x, float y, const wchar_t* label, float& value, float min, float max);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND m_hWnd;
    HWND m_hTargetWnd;
    ID2D1Factory* m_pD2DFactory;
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_pBrush;
    IDWriteFactory* m_pDWriteFactory;
    IDWriteTextFormat* m_pTextFormat;
    
    std::thread m_renderThread;
    std::atomic<bool> m_running;
};
