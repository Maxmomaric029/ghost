#include "ESPRenderer.h"
#include <string>

void ESPRenderer::Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const EntityCache& cache) {
    for (const auto& entity : cache.GetEntities()) {
        Vector2 footPos, headPos;
        if (WorldToScreen(entity.position, footPos) && WorldToScreen(entity.headPosition, headPos)) {
            float height = abs(footPos.y - headPos.y);
            float width = height / 2.0f;

            brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
            rt->DrawRectangle(D2D1::RectF(footPos.x - width / 2.0f, headPos.y, footPos.x + width / 2.0f, footPos.y), brush);

            rt->DrawLine(D2D1::Point2F(rt->GetSize().width / 2.0f, rt->GetSize().height), D2D1::Point2F(footPos.x, footPos.y), brush);

            std::wstring info = L"HP: " + std::to_wstring((int)entity.health);
            rt->DrawTextW(info.c_str(), (UINT32)info.length(), textFormat, D2D1::RectF(footPos.x - width / 2.0f, headPos.y - 20, footPos.x + width / 2.0f, headPos.y), brush);
        }
    }
}

bool ESPRenderer::WorldToScreen(Vector3 worldPos, Vector2& screenPos) {
    // Simplified projection logic for the purpose of the DLL template
    // In a real implementation, matrix multiplication with Projection and ModelView is required
    screenPos.x = 960.0f; // Center screen placeholder
    screenPos.y = 540.0f;
    return true; 
}
