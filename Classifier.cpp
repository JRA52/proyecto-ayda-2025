#include "Classifier.hpp"
#include <iostream>
#include <map>
#include <algorithm>
#include <cmath>
#include <vector>

const double PI = 3.14159265358979323846;

int g_total_neu = 0;
int g_total_mon = 0;
int g_total_lin = 0;
int g_total_eos = 0;
int g_total_atp = 0; 

struct Cluster 
{
    std::vector<int> ids;
    int area = 0;
    double x = 0, y = 0;
    double circ_sum = 0;
    std::map<SegmentType, int> neighbors;
};

SegmentType CellClassifier::classify_color(double r, double g, double b, int size) 
{
    if (size < 3)
    {
        return SegmentType::BACKGROUND; 
    }    
    if (size > 1500)
    {
        return SegmentType::BACKGROUND;
    }  

    double intensity = r + g + b;

    if (intensity < 50)
    {
        return SegmentType::BACKGROUND; 
    }  
    if (intensity > 610)
    {
        return SegmentType::BACKGROUND;
    }  

    bool is_dark = (intensity < 550); 
    bool is_violet = (g < r - 5) && (g < b - 5); 
    
    if (is_dark && is_violet)
    {
        return SegmentType::CORE;
    } 

    if (r > b + 45 && r > g + 45)
    {
        return SegmentType::EOS_GRANULES;
    } 
    
    if (intensity < 680 && (b > r || std::abs(r-b) < 20) && std::abs(r-g) > 10) 
    {
        return SegmentType::LYMPHOCYTE_CYTOPLASM; 
    }

    return SegmentType::BACKGROUND; 
}

<<<<<<< Updated upstream
void CellClassifier::analizar_segments(Designar::SLList<GraphType>& segments, int width, int height) 
=======
void CellClassifier::analyze_segments(Designar::SLList<GraphType>& segments, int width, int height) 
>>>>>>> Stashed changes
{
    pixel_segment_map.assign(height, std::vector<int>(width, -1));
    stats_list.clear();

    int seg_id = 0;
    segments.for_each([&](GraphType& g) 
    {
        double sr=0, sg=0, sb=0;
        int count = 0;
        int min_x = width, max_x = 0, min_y = height, max_y = 0;

        g.for_each_node([&](Designar::Node<GraphType>* n) 
        {
            auto& info = n->get_info();
            sr += info.r; sg += info.g; sb += info.b;
            count++;
            if(info.x < min_x) min_x = info.x;
            if(info.x > max_x) max_x = info.x;
            if(info.y < min_y) min_y = info.y;
            if(info.y > max_y) max_y = info.y;
        });

        if (count > 0) 
        {
            SegmentStats st;
            st.id = seg_id;
            st.size = count;
            st.r = sr/count; st.g = sg/count; st.b = sb/count;
            st.min_x = min_x; st.max_x = max_x; st.min_y = min_y; st.max_y = max_y;
            st.type = classify_color(st.r, st.g, st.b, st.size);
            st.perimeter = 0;
            st.circularity = 0.0; 
            stats_list.push_back(st);

<<<<<<< Updated upstream
            if (st.tipo != SegmentType::BACKGROUND) 
=======
            if (st.type != SegmentType::BACKGROUND) 
>>>>>>> Stashed changes
            {
                g.for_each_node([&](Designar::Node<GraphType>* n) 
                {
                    pixel_segment_map[n->get_info().y][n->get_info().x] = st.id;
                });
            }
        }
        seg_id++;
    });

    for (auto& st : stats_list) 
    {
<<<<<<< Updated upstream
        if (st.tipo == SegmentType::BACKGROUND) continue;
=======
        if (st.type == SegmentType::BACKGROUND) continue;
>>>>>>> Stashed changes
        int perimeter = 0;
        for (int y = st.min_y; y <= st.max_y; ++y) 
        {
            for (int x = st.min_x; x <= st.max_x; ++x) 
            {
                if (pixel_segment_map[y][x] == st.id) 
                {
                    bool is_border = false;
                    if (x==0 || x==width-1 || y==0 || y==height-1)
                    {
                        is_border = true;
                    } 
                    else if (pixel_segment_map[y][x-1] != st.id)
                    {
                        is_border = true;
                    } 
                    else if (pixel_segment_map[y][x+1] != st.id)
                    {
                        is_border = true;
                    } 
                    else if (pixel_segment_map[y-1][x] != st.id)
                    {
                        is_border = true;
                    } 
                    else if (pixel_segment_map[y+1][x] != st.id)
                    {
                        is_border = true;
                    } 
                    if (is_border)
                    {
                        perimeter++;
                    } 
                }
            }
        }
        st.perimeter = perimeter;
        if (perimeter > 0) st.circularity = (4.0 * PI * (double)st.size) / ((double)perimeter * perimeter);
    }
}

void CellClassifier::count_cells() 
{
    std::vector<bool> processed(stats_list.size(), false);
    std::vector<Cluster> cells;

    double MAX_DISTANCE = 7.5;  
    int MIN_TAM_CORE = 5;   

    for (size_t i = 0; i < stats_list.size(); ++i) 
    {
<<<<<<< Updated upstream
        if (stats_list[i].tipo != SegmentType::CORE)
=======
        if (stats_list[i].type != SegmentType::CORE)
>>>>>>> Stashed changes
        {
            continue;
        } 
        if (stats_list[i].size < MIN_TAM_CORE)
        {
            continue;
        }  
        if (processed[i])
        {
            continue;
        } 

        Cluster c;
        c.ids.push_back(i);
        processed[i] = true;

        bool swap = true;
        while(swap) 
        {
            swap = false;
            for (size_t j = 0; j < stats_list.size(); ++j) 
            {
<<<<<<< Updated upstream
                if (stats_list[j].tipo != SegmentType::CORE || processed[j] || stats_list[j].size < MIN_TAM_CORE) continue;
=======
                if (stats_list[j].type != SegmentType::CORE || processed[j] || stats_list[j].size < MIN_TAM_CORE) continue;
>>>>>>> Stashed changes

                for (int id_c : c.ids) 
                {
                    auto& s1 = stats_list[id_c];
                    auto& s2 = stats_list[j];

                    double cx1 = (s1.min_x + s1.max_x)/2.0, cy1 = (s1.min_y + s1.max_y)/2.0;
                    double cx2 = (s2.min_x + s2.max_x)/2.0, cy2 = (s2.min_y + s2.max_y)/2.0;
                    double dist = std::sqrt(std::pow(cx1-cx2, 2) + std::pow(cy1-cy2, 2));

                    if (dist < MAX_DISTANCE) 
                    {
                        c.ids.push_back(j);
                        processed[j] = true;
                        swap = true;
                        break; 
                    }
                }
            }
        }

        double accumulated_circle = 0;
        for (int id : c.ids) 
        {
            auto& s = stats_list[id];
            c.area += s.size;
            accumulated_circle += s.circularity;
            
            int range = 4;
            for (int y = std::max(0, s.min_y - range); y <= std::min((int)pixel_segment_map.size()-1, s.max_y + range); ++y) 
            {
                for (int x = std::max(0, s.min_x - range); x <= std::min((int)pixel_segment_map[0].size()-1, s.max_x + range); ++x) 
                {
                    int v_id = pixel_segment_map[y][x];
                    if (v_id != -1 && v_id != s.id) c.neighbors[stats_list[v_id].type]++;
                }
            }
        }
        c.circ_sum = accumulated_circle / c.ids.size();
        cells.push_back(c);
    }

    int n_neu=0, n_eos=0, n_lin=0, n_mon=0;
    
    std::cout << "\n--- CLUSTER ANALYSIS ---" << std::endl;

    for (auto& c : cells) 
    {
        if (c.area < 35)
        {
            continue; 
        } 

        int n_parts = c.ids.size();
        std::string diag;

<<<<<<< Updated upstream
        if (c.vecinos[SegmentType::EOS_GRANULES] > 40) 
=======
        if (c.neighbors[SegmentType::EOS_GRANULES] > 40) 
>>>>>>> Stashed changes
        { 
            n_eos++; diag = "EOSINOPHIL";
        }
        else if (c.area > 140) 
        {
            n_mon++; diag = "MONOCYTE";
        }

        else 
        {  
            if (n_parts >= 3) 
            {  
                if (c.area > 55) 
                {
                    n_neu++; diag = "NEUTROPHIL";
                } else 
                {  
                    n_lin++; diag = "LYMPHOCYTE";
                }
            }
        }
        
    }

    std::cout << "\n=== FINAL COUNT ===" << std::endl;
    std::cout << "NEUTROPHILS: " << n_neu << std::endl;
    std::cout << "MONOCYTES:   " << n_mon << std::endl;
    std::cout << "LYMPHOCYTES:  " << n_lin << std::endl;
    std::cout << "EOSINOPHILS: " << n_eos << std::endl;
    std::cout << "TOTAL:       " << (n_neu + n_mon + n_lin + n_eos) << std::endl;

    g_total_neu += n_neu;
    g_total_mon += n_mon;
    g_total_lin += n_lin;
    g_total_eos += n_eos;
   
}