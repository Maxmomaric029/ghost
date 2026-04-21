#pragma once
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <thread>
#include <mutex>
#include <atomic>

class OverlayWindow {
public:
    OverlayWindow();
    ~OverlayWindow();

    bool Create(HWND hTarget);
    void Destroy();
    void Run();
    void UpdatePosition();
    void OnPaint();
    void Invalidate() { if (m_hWnd) InvalidateRect(m_hWnd, NULL, FALSE); }

    ID2D1HwndRenderTarget* GetRT() { return m_pRenderTarget; }
    ID2D1SolidColorBrush* GetBrush() { return m_pBrush; }
    IDWriteTextFormat* GetTextFormat() { return m_pTextFormat; }

private:
    HWND m_hWnd;
    HWND m_hTargetWnd;
    ID2D1Factory* m_pD2DFactory;
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_pBrush;
    IDWriteFactory* m_pDWriteFactory;
    IDWriteTextFormat* m_pTextFormat;

    std::thread m_renderThread;
    std::atomic<bool> m_running;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
