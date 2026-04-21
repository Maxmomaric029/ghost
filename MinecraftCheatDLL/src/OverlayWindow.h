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

    bool Create(HWND hTargetWnd);
    void Destroy();
    void UpdatePosition();
    void Render();
    void Invalidate() { m_needsRedraw = true; }

    ID2D1HwndRenderTarget* GetRT() { return m_pRT; }
    ID2D1SolidColorBrush* GetBrush() { return m_pBrush; }
    IDWriteTextFormat* GetTextFormat() { return m_pTextFormat; }

private:
    HWND m_hWnd;
    HWND m_targetWnd;
    ID2D1Factory* m_pFactory;
    ID2D1HwndRenderTarget* m_pRT;
    ID2D1SolidColorBrush* m_pBrush;
    IDWriteFactory* m_pWriteFactory;
    IDWriteTextFormat* m_pTextFormat;

    std::thread m_overlayThread;
    std::atomic<bool> m_running;
    std::atomic<bool> m_needsRedraw;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void OverlayThreadFunc();
};
