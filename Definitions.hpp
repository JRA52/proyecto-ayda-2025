#pragma once

#include <cmath>

enum class SegmentType 
{
    FONDO,
    NUCLEO,
    GRANULO_EOS,            // Rojo
    CITOPLASMA_NEUTRO,      // Rosa pálido
    CITOPLASMA_LINFOCITO,   // Azul claro brillante
    CITOPLASMA_MONOCITO,    // Gris azulado 
    DESCONOCIDO
};

struct PixelInfo 
{
    int x, y;
    unsigned char r, g, b;
    int segmento_id = -1; 
};

struct EdgeInfo 
{
    double weight; 
};

inline double color_distance(const PixelInfo& p1, const PixelInfo& p2)
{
    double dr = (double)p1.r - p2.r;
    double dg = (double)p1.g - p2.g;
    double db = (double)p1.b - p2.b;
    return std::sqrt(dr*dr + dg*dg + db*db);
}