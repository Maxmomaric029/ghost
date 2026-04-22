#include "ESPRenderer.h"
#include "CheatCore.h"
#include "Utils.h"
#include <string>

void ESPRenderer::Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, EntityCache& cache) {
    auto entities = cache.GetEntities();
    D2D1_SIZE_F size = rt->GetSize();

    for (const auto& entity : entities) {
        Vector2 screenPos;
        // Obtenemos las matrices directamente de CheatCore
        if (Utils::WorldToScreen(entity.pos, CheatCore::Instance().GetViewMatrix(), CheatCore::Instance().GetProjMatrix(), (int)size.width, (int)size.height, screenPos)) {
            
            brush->SetColor(D2D1::ColorF(0.0f, 1.0f, 0.5f, 1.0f));
            
            float width = 40.0f;
            float height = 80.0f;
            
            // Box
            rt->DrawRectangle(D2D1::RectF(screenPos.x - width/2, screenPos.y - height, screenPos.x + width/2, screenPos.y), brush, 1.5f);
            
            // Health bar
            float healthPercent = std::clamp(entity.health / 20.0f, 0.0f, 1.0f);
            float barHeight = height * healthPercent;
            float barX = screenPos.x - width/2 - 6;
            
            brush->SetColor(D2D1::ColorF(1.0f - healthPercent, healthPercent, 0.0f, 1.0f)); // Rojo a Verde
            rt->FillRectangle(D2D1::RectF(barX, screenPos.y - barHeight, barX + 4, screenPos.y), brush);
            
            // Info text (Distance + HP)
            float dist = (float)CheatCore::Instance().GetLocalPos().Distance(entity.pos);
            std::wstring info = L"[" + std::to_wstring((int)entity.health) + L"hp | " + std::to_wstring((int)dist) + L"m]";
            
            brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
            rt->DrawTextW(info.c_str(), (UINT32)info.length(), textFormat, D2D1::RectF(screenPos.x - 50, screenPos.y - height - 20, screenPos.x + 150, screenPos.y - height), brush);
        }
    }
}
