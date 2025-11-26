#include "ImageGraph.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool ImageGraphBuilder::load_and_build(const std::string& filename) 
{
    int raw_width, raw_height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &raw_width, &raw_height, &channels, 0);

    if (!data) 
    {
<<<<<<< Updated upstream
        std::cerr << "Error loading image: " << filename << std::endl;
=======
        std::cerr << "Error while loading image: " << filename << std::endl;
>>>>>>> Stashed changes
        return false;
    }

    if (channels < 3) 
    {
        std::cerr << "The image must be RGB" << std::endl;
        stbi_image_free(data);
        return false;
    }

    const int STEP = 2; 

    this->width = raw_width / STEP;
    this->height = raw_height / STEP;

    node_grid.resize(height, std::vector<NodePtr>(width, nullptr));

    for (int y = 0; y < height; ++y) 
    {
        for (int x = 0; x < width; ++x) 
        {
            
            int org_x = x * STEP;
            int org_y = y * STEP;

            int r_sum = 0, g_sum = 0, b_sum = 0, count = 0;

            for (int dy = 0; dy < STEP; ++dy) 
            {
                for (int dx = 0; dx < STEP; ++dx) 
                {
                    if (org_x + dx < raw_width && org_y + dy < raw_height) 
                    {
                        unsigned char* p = data + ((org_y + dy) * raw_width + (org_x + dx)) * channels;
                        r_sum += p[0];
                        g_sum += p[1];
                        b_sum += p[2];
                        count++;
                    }
                }
            }

            PixelInfo info;
            info.x = x; 
            info.y = y;
            info.r = (unsigned char)(r_sum / count);
            info.g = (unsigned char)(g_sum / count);
            info.b = (unsigned char)(b_sum / count);
            info.segment_id = -1;

            node_grid[y][x] = graph.insert_node(info);
        }
    }

    stbi_image_free(data); 

    for (int y = 0; y < height; ++y) 
    {
        for (int x = 0; x < width; ++x) 
        {
            NodePtr p = node_grid[y][x];

            if (x + 1 < width) 
            {
                NodePtr q = node_grid[y][x+1];
                double w = color_distance(p->get_info(), q->get_info());
                graph.insert_arc(p, q, {w});
            }

            if (y + 1 < height) 
            {
                NodePtr q = node_grid[y+1][x];
                double w = color_distance(p->get_info(), q->get_info());
                graph.insert_arc(p, q, {w});
            }
        }
    }
    return true;
}