#pragma once
#include "ImageGraph.hpp"
#include <vector>

struct SegmentStats 
{
    int id;
    SegmentType tipo;
    int size;           
    
    int perimeter;  
    double circularity;

    double r, g, b; 
    int min_x, max_x, min_y, max_y; 
};

class CellClassifier 
{
    std::vector<SegmentStats> stats_list;
    std::vector<std::vector<int>> pixel_segment_map;

public:
    void analizar_segmentos(Designar::SLList<GraphType>& segmentos, int width, int height);
    void contar_celulas();
    
private:
    SegmentType clasificar_color(double r, double g, double b, int size);
};