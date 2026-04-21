#include "Utils.h"

namespace Utils {
    /**
     * Implementación completa de WorldToScreen con división por W.
     * Esencial para el ESP preciso.
     */
    bool WorldToScreen(const Vector3& worldPos, const Matrix4x4& viewMatrix, const Matrix4x4& projMatrix, int width, int height, Vector2& screenPos) {
        float w = 0.0f;
        
        // Transformación: Mundo -> Vista
        Vector3 viewSpace = viewMatrix.Multiply(worldPos, w);
        
        // Transformación: Vista -> Clip (Proyección)
        Vector3 clipSpace = projMatrix.Multiply(viewSpace, w);

        // Si el objeto está detrás de la cámara, no dibujamos
        if (w < 0.1f) return false;

        // Normalizar coordenadas (NDC) y convertir a espacio de pantalla (píxeles)
        Vector2 ndc = { (float)clipSpace.x / w, (float)clipSpace.y / w };
        
        screenPos.x = (width / 2.0f) + (ndc.x * width / 2.0f);
        screenPos.y = (height / 2.0f) - (ndc.y * height / 2.0f);
        
        return true;
    }

    // Más funciones matemáticas para el sistema de predicción de arco
    // ...
}
