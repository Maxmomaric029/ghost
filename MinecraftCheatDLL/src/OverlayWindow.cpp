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

void OverlayWindow::Invalidate() {
    if (m_hWnd) InvalidateRect(m_hWnd, NULL, FALSE);
}

void OverlayWindow::SetClickThrough(bool enabled) {
    if (!m_hWnd) return;
    LONG style = GetWindowLong(m_hWnd, GWL_EXSTYLE);
    if (enabled) style |= WS_EX_TRANSPARENT;
    else style &= ~WS_EX_TRANSPARENT;
    SetWindowLong(m_hWnd, GWL_EXSTYLE, style);
}

void OverlayWindow::Run() {
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"MC_GHOST_OVERLAY", NULL };
    RegisterClassExW(&wc);

    RECT rect;
    GetWindowRect(m_hTargetWnd, &rect);
    m_hWnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW, L"MC_GHOST_OVERLAY", L"Ghost Overlay", WS_POPUP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, GetModuleHandle(NULL), this);
    
    SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 255, LWA_COLORKEY);
    const MARGINS margins = { -1, -1, -1, -1 };
    DwmExtendFrameIntoClientArea(m_hWnd, &margins);

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)), &m_pRenderTarget);
    m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pBrush);

    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pDWriteFactory);
    m_pDWriteFactory->CreateTextFormat(L"Segoe UI", NULL, DWRITE_FONT_WEIGHT_MEDIUM, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &m_pTextFormat);

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
    SetWindowPos(m_hWnd, HWND_TOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE);
}

void OverlayWindow::OnPaint() {
    if (!m_pRenderTarget) return;
    m_pRenderTarget->BeginDraw();
    m_pRenderTarget->Clear(D2D1::ColorF(0, 0, 0, 0));
    
    CheatCore::Instance().DrawESP(m_pRenderTarget, m_pBrush, m_pTextFormat);

    if (CheatCore::Instance().IsMenuVisible()) {
        DrawMenu(m_pRenderTarget);
    }

    m_pRenderTarget->EndDraw();
}

void OverlayWindow::DrawMenu(ID2D1RenderTarget* rt) {
    D2D1_SIZE_F size = rt->GetSize();
    float w = 350.0f;
    float h = 400.0f;
    float x = (size.width - w) / 2.0f;
    float y = (size.height - h) / 2.0f;

    // Fondo del Menú (Dark Premium)
    m_pBrush->SetColor(D2D1::ColorF(0.08f, 0.08f, 0.1f, 0.95f));
    rt->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x, y, x + w, y + h), 12.0f, 12.0f), m_pBrush);
    
    // Borde brillante con degradado (simulado con color fijo para velocidad)
    m_pBrush->SetColor(D2D1::ColorF(0.4f, 0.3f, 1.0f, 1.0f));
    rt->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x, y, x + w, y + h), 12.0f, 12.0f), m_pBrush, 2.5f);

    // Cabecera
    m_pBrush->SetColor(D2D1::ColorF(0.15f, 0.15f, 0.2f, 1.0f));
    rt->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x, y, x + w, y + 60), 12.0f, 12.0f), m_pBrush);
    
    m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    rt->DrawTextW(L"ANTIGRAVITY GHOST", 18, m_pTextFormat, D2D1::RectF(x + 20, y + 20, x + w, y + 60), m_pBrush);

    // Toggles
    DrawToggle(rt, x + 30, y + 80, L"Aimbot (Auto-Aim)", CheatCore::Instance().GetAimbotEnabled());
    DrawToggle(rt, x + 30, y + 130, L"ESP (Visual Box)", CheatCore::Instance().GetESPEnabled());
    
    m_pBrush->SetColor(D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f));
    rt->DrawTextW(L"Presiona INSERT para ocultar", 28, m_pTextFormat, D2D1::RectF(x + 30, y + h - 40, x + w, y + h), m_pBrush);
}

void OverlayWindow::DrawToggle(ID2D1RenderTarget* rt, float x, float y, const wchar_t* label, bool enabled) {
    float toggleWidth = 40.0f;
    float toggleHeight = 20.0f;

    // Etiqueta
    m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    rt->DrawTextW(label, (UINT32)wcslen(label), m_pTextFormat, D2D1::RectF(x, y, x + 250, y + 30), m_pBrush);

    // Fondo Toggle
    if (enabled) m_pBrush->SetColor(D2D1::ColorF(0.3f, 0.7f, 0.3f, 1.0f));
    else m_pBrush->SetColor(D2D1::ColorF(0.3f, 0.3f, 0.3f, 1.0f));
    
    rt->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x + 260, y, x + 260 + toggleWidth, y + toggleHeight), 10.0f, 10.0f), m_pBrush);
    
    // Círculo Toggle
    m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    float circleX = enabled ? (x + 260 + toggleWidth - 10) : (x + 260 + 10);
    rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(circleX, y + 10), 8, 8), m_pBrush);
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
            case WM_NCHITTEST: 
                if (CheatCore::Instance().IsMenuVisible()) return HTCLIENT;
                return HTTRANSPARENT;
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
