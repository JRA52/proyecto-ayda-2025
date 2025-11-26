#include "Segmenter.hpp"
#include <iostream>
#include <vector>
#include <map>

using AdArcIt = Designar::AdArcIt<GraphType>;

Designar::SLList<GraphType> GraphSegmenter::segment(const GraphType& source_graph, double threshold) 
{
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    //int broken_cells = 0;
    
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
    for (Designar::ArcIt<GraphType> it(source_graph); it.has_current(); it.next()) 
    {
        auto* a = it.get_current();
        
        if (a->get_info().weight > threshold) 
        {
<<<<<<< Updated upstream
<<<<<<< Updated upstream
            a->visit(Designar::GraphTag::COMPONENT); 
            //broken_cells++;
=======
            a->visit(Designar::GraphTag::COMPONENT);           
>>>>>>> Stashed changes
=======
            a->visit(Designar::GraphTag::COMPONENT);           
>>>>>>> Stashed changes
        } 
        else 
        {
            a->unvisit(Designar::GraphTag::COMPONENT);
        }
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    }
    //std::cout << "Conexiones cortadas: " << broken_cells << std::endl;
=======
    }    
>>>>>>> Stashed changes
=======
    }    
>>>>>>> Stashed changes

    Designar::SLList<GraphType> segments;
    
    source_graph.reset_node_cookies();
    for (Designar::NodeIt<GraphType> it(source_graph); it.has_current(); it.next()) 
    {
        it.get_current()->unvisit(Designar::GraphTag::COMPONENT);
    }

    for (Designar::NodeIt<GraphType> it(source_graph); it.has_current(); it.next()) 
    {
        auto* seed = it.get_current();

        if (seed->is_visited(Designar::GraphTag::COMPONENT))
        {
            continue;
        } 

        GraphType subgraph;
        
        std::vector<Designar::Node<GraphType>*> queue;
        
        queue.push_back(seed);
        seed->visit(Designar::GraphTag::COMPONENT);

        std::map<Designar::Node<GraphType>*, Designar::Node<GraphType>*> node_map;

        node_map[seed] = subgraph.insert_node(seed->get_info());

        size_t head = 0;
        while(head < queue.size()) 
        {
            auto* u = queue[head++];
            auto* u_sub = node_map[u];

            for (AdArcIt it_adj(source_graph, u); it_adj.has_current(); it_adj.next()) 
            {
                auto* arc = it_adj.get_current();

                if (arc->is_visited(Designar::GraphTag::COMPONENT)) continue;

                auto* v = it_adj.get_tgt_node();
                
                if (!v->is_visited(Designar::GraphTag::COMPONENT)) 
                {
                    v->visit(Designar::GraphTag::COMPONENT);
                    queue.push_back(v);
                    
                    auto* v_sub = subgraph.insert_node(v->get_info());
                    node_map[v] = v_sub;
                    subgraph.insert_arc(u_sub, v_sub, arc->get_info());
                } 
                else if (node_map.find(v) != node_map.end()) 
                {
                     if (subgraph.search_arc(u_sub, node_map[v]) == nullptr) 
                     {
                        subgraph.insert_arc(u_sub, node_map[v], arc->get_info());
                     }
                }
            }
        }
        
        segments.append(std::move(subgraph));
    }

    return segments;
}