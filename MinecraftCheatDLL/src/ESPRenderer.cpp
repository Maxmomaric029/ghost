#include "ESPRenderer.h"
#include "Utils.h"
#include <string>

// Simulación de obtención de matrices para el stress test
// En una implementación final, estas se leerían vía JNI desde GameRenderer
static Matrix4x4 dummyView = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
static Matrix4x4 dummyProj = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

void ESPRenderer::Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, EntityCache& cache) {
    auto entities = cache.GetEntities();
    D2D1_SIZE_F size = rt->GetSize();

    for (const auto& entity : entities) {
        Vector2 screenPos;
        if (Utils::WorldToScreen(entity.pos, dummyView, dummyProj, (int)size.width, (int)size.height, screenPos)) {
            
            brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
            
            float width = 40.0f;
            float height = 80.0f;
            
            rt->DrawRectangle(D2D1::RectF(screenPos.x - width/2, screenPos.y - height, screenPos.x + width/2, screenPos.y), brush);
            
            std::wstring info = L"Enemigo [" + std::to_wstring((int)entity.health) + L" HP]";
            rt->DrawTextW(info.c_str(), (UINT32)info.length(), textFormat, D2D1::RectF(screenPos.x - width/2, screenPos.y - height - 20, screenPos.x + 200, screenPos.y - height), brush);
        }
    }
}
