#include "ESPRenderer.h"
#include <string>

void ESPRenderer::Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, EntityCache& cache) {
    auto entities = cache.GetEntities();
    for (const auto& entity : entities) {
        // Simplified WorldToScreen for this stress test template
        // In a real DLL, we'd multiply by the projection matrix here
        float screenX = 960.0f; 
        float screenY = 540.0f;

        brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
        rt->DrawRectangle(D2D1::RectF(screenX - 20, screenY - 40, screenX + 20, screenY + 40), brush);
        
        std::wstring info = L"HP: " + std::to_wstring((int)entity.health);
        rt->DrawTextW(info.c_str(), (UINT32)info.length(), textFormat, D2D1::RectF(screenX - 20, screenY - 60, screenX + 100, screenY - 40), brush);
    }
}
