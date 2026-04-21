#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include "EntityCache.h"

class ESPRenderer {
public:
    static void Draw(ID2D1RenderTarget* rt, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, const EntityCache& cache);
private:
    static bool WorldToScreen(Vector3 worldPos, Vector2& screenPos);
};
