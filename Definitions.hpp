#pragma once

#include <cmath>

//PONER NOMBRES EN INGLES
enum class SegmentType 
{
    BACKGROUND,
    CORE,
    EOS_GRANULES,            // RED
    NEUTRAL_CYTOPLASM,       // Pale pink
    LYMPHOCYTE_CYTOPLASM,    // Bright light blue
    MONOCYTE_CYTOPLASM,      // bluish gray
    UNKNOWN
};



struct PixelInfo 
{
    int x, y;
    unsigned char r, g, b;
    int segment_id = -1; 
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