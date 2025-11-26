#pragma once
#include "ImageGraph.hpp"
#include <vector>

struct SegmentStats 
{
    int id;
    SegmentType type;
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
    void analyze_segments(Designar::SLList<GraphType>& segments, int width, int height);
    void count_cells();
    
private:
    SegmentType classify_color(double r, double g, double b, int size);
};