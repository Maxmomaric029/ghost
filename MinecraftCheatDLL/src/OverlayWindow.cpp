#include "OverlayWindow.h"
#include <dwmapi.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dwmapi.lib")

OverlayWindow::OverlayWindow() : m_hWnd(NULL), m_targetWnd(NULL), m_pFactory(NULL), m_pRT(NULL), m_pBrush(NULL), m_pWriteFactory(NULL), m_pTextFormat(NULL), m_running(false), m_needsRedraw(false) {}

OverlayWindow::~OverlayWindow() { Destroy(); }

bool OverlayWindow::Create(HWND hTargetWnd) {
    m_targetWnd = hTargetWnd;
    m_running = true;
    m_overlayThread = std::thread(&OverlayWindow::OverlayThreadFunc, this);
    return true;
}

void OverlayWindow::Destroy() {
    m_running = false;
    if (m_overlayThread.joinable()) m_overlayThread.join();
    if (m_pTextFormat) m_pTextFormat->Release();
    if (m_pWriteFactory) m_pWriteFactory->Release();
    if (m_pBrush) m_pBrush->Release();
    if (m_pRT) m_pRT->Release();
    if (m_pFactory) m_pFactory->Release();
}

void OverlayWindow::UpdatePosition() {
    RECT rect;
    GetWindowRect(m_targetWnd, &rect);
    SetWindowPos(m_hWnd, HWND_TOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
    if (m_pRT) {
        m_pRT->Resize(D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top));
    }
}

void OverlayWindow::OverlayThreadFunc() {
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"MC_OVERLAY", NULL };
    RegisterClassExW(&wc);

    RECT rect;
    GetWindowRect(m_targetWnd, &rect);
    m_hWnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW, L"MC_OVERLAY", L"Overlay", WS_POPUP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, GetModuleHandle(NULL), this);
    
    SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    const MARGINS margins = { -1, -1, -1, -1 };
    DwmExtendFrameIntoClientArea(m_hWnd, &margins);

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
    m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)), &m_pRT);
    m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pBrush);

    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pWriteFactory);
    m_pWriteFactory->CreateTextFormat(L"Consolas", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &m_pTextFormat);

    ShowWindow(m_hWnd, SW_SHOW);

    MSG msg;
    while (m_running) {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        UpdatePosition();
        if (m_needsRedraw) {
            Render();
            m_needsRedraw = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    DestroyWindow(m_hWnd);
}

void OverlayWindow::Render() {
    if (!m_pRT) return;
    m_pRT->BeginDraw();
    m_pRT->Clear(D2D1::ColorF(0, 0, 0, 0));
    // ESPRenderer::Draw will be called from CheatCore
    m_pRT->EndDraw();
}

LRESULT CALLBACK OverlayWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCHITTEST) return HTTRANSPARENT;
    if (msg == WM_PAINT) { ValidateRect(hWnd, NULL); return 0; }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
