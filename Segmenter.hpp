#pragma once
#include <list.hpp> 
#include <graphalgorithms.hpp>
#include "ImageGraph.hpp"

struct PixelDistance 
{
    using Type = double;
    static constexpr Type ZERO = 0.0;
    static constexpr Type MAX = 1e9; 
    Type operator()(Designar::Arc<GraphType>* a) const 
    {
        return a->get_info().weight;
    }
};

class GraphSegmenter 
{
public:

    Designar::SLList<GraphType> segmentar(const GraphType& source_graph, double umbral);
};