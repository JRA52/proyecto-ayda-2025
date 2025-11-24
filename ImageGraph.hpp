#pragma once
#include <vector>
#include <string>
#include <graph.hpp> 
#include "Definitions.hpp"

using GraphType = Designar::Graph<PixelInfo, EdgeInfo>;
using NodePtr = Designar::Node<GraphType>*;

class ImageGraphBuilder 
{
    int width, height;
public:
    GraphType graph;
    std::vector<std::vector<NodePtr>> node_grid;

    bool load_and_build(const std::string& filename);
    int get_width() const { return width; }
    int get_height() const { return height; }
};