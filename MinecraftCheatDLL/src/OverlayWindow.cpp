#include "OverlayWindow.h"
#include "CheatCore.h"
#include <dwmapi.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dwmapi.lib")

OverlayWindow::OverlayWindow() : m_hWnd(NULL), m_hTargetWnd(NULL), m_pD2DFactory(NULL), m_pRenderTarget(NULL), m_pBrush(NULL), m_pDWriteFactory(NULL), m_pTextFormat(NULL), m_running(false) {}

OverlayWindow::~OverlayWindow() { Destroy(); }

bool OverlayWindow::Create(HWND hTarget) {
    m_hTargetWnd = hTarget;
    m_running = true;
    m_renderThread = std::thread(&OverlayWindow::Run, this);
    return true;
}

void OverlayWindow::Destroy() {
    m_running = false;
    if (m_hWnd) PostMessage(m_hWnd, WM_CLOSE, 0, 0);
    if (m_renderThread.joinable()) m_renderThread.join();
    
    if (m_pTextFormat) m_pTextFormat->Release();
    if (m_pDWriteFactory) m_pDWriteFactory->Release();
    if (m_pBrush) m_pBrush->Release();
    if (m_pRenderTarget) m_pRenderTarget->Release();
    if (m_pD2DFactory) m_pD2DFactory->Release();
}

void OverlayWindow::Run() {
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"MC_GHOST_OVERLAY", NULL };
    RegisterClassExW(&wc);

    RECT rect;
    GetWindowRect(m_hTargetWnd, &rect);
    m_hWnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW, L"MC_GHOST_OVERLAY", L"Ghost Overlay", WS_POPUP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, GetModuleHandle(NULL), this);
    
    SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    const MARGINS margins = { -1, -1, -1, -1 };
    DwmExtendFrameIntoClientArea(m_hWnd, &margins);

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)), &m_pRenderTarget);
    m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pBrush);

    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pDWriteFactory);
    m_pDWriteFactory->CreateTextFormat(L"Consolas", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &m_pTextFormat);

    ShowWindow(m_hWnd, SW_SHOW);

    MSG msg;
    while (m_running && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        UpdatePosition();
    }
}

void OverlayWindow::UpdatePosition() {
    if (!m_hTargetWnd || !m_hWnd) return;
    RECT rect;
    GetWindowRect(m_hTargetWnd, &rect);
    SetWindowPos(m_hWnd, HWND_TOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
}

void OverlayWindow::OnPaint() {
    if (!m_pRenderTarget) return;
    m_pRenderTarget->BeginDraw();
    m_pRenderTarget->Clear(D2D1::ColorF(0, 0, 0, 0));
    
    CheatCore::Instance().DrawESP(m_pRenderTarget, m_pBrush, m_pTextFormat);

    m_pRenderTarget->EndDraw();
}

LRESULT CALLBACK OverlayWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    OverlayWindow* pThis = nullptr;
    if (msg == WM_NCCREATE) {
        pThis = (OverlayWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    } else {
        pThis = (OverlayWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (pThis) {
        switch (msg) {
            case WM_PAINT: pThis->OnPaint(); return 0;
            case WM_ERASEBKGND: return 1;
            case WM_NCHITTEST: return HTTRANSPARENT;
            case WM_SIZE:
                if (pThis->m_pRenderTarget) {
                    RECT r; GetClientRect(hWnd, &r);
                    pThis->m_pRenderTarget->Resize(D2D1::SizeU(r.right, r.bottom));
                }
                return 0;
        }
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
