#include "Segmenter.hpp"
#include <iostream>
#include <vector>
#include <map>

using AdArcIt = Designar::AdArcIt<GraphType>;

Designar::SLList<GraphType> GraphSegmenter::segmentar(const GraphType& source_graph, double umbral) 
{
    std::cout << "Iniciando segmentacion rapida (Iterativa BFS Correcta)..." << std::endl;

    int rotos = 0;
    
    for (Designar::ArcIt<GraphType> it(source_graph); it.has_current(); it.next()) 
    {
        auto* a = it.get_current();
        
        if (a->get_info().weight > umbral) 
        {
            a->visit(Designar::GraphTag::COMPONENT); 
            rotos++;
        } 
        else 
        {
            a->unvisit(Designar::GraphTag::COMPONENT);
        }
    }
    std::cout << "Conexiones cortadas: " << rotos << std::endl;

    Designar::SLList<GraphType> segmentos;
    
    source_graph.reset_node_cookies();
    for (Designar::NodeIt<GraphType> it(source_graph); it.has_current(); it.next()) 
    {
        it.get_current()->unvisit(Designar::GraphTag::COMPONENT);
    }

    for (Designar::NodeIt<GraphType> it(source_graph); it.has_current(); it.next()) 
    {
        auto* semilla = it.get_current();

        if (semilla->is_visited(Designar::GraphTag::COMPONENT))
        {
            continue;
        } 

        GraphType subgrafo;
        
        std::vector<Designar::Node<GraphType>*> cola;
        
        cola.push_back(semilla);
        semilla->visit(Designar::GraphTag::COMPONENT);

        std::map<Designar::Node<GraphType>*, Designar::Node<GraphType>*> mapa_nodos;

        mapa_nodos[semilla] = subgrafo.insert_node(semilla->get_info());

        size_t head = 0;
        while(head < cola.size()) 
        {
            auto* u = cola[head++];
            auto* u_sub = mapa_nodos[u];

            for (AdArcIt it_adj(source_graph, u); it_adj.has_current(); it_adj.next()) 
            {
                auto* arco = it_adj.get_current();

                if (arco->is_visited(Designar::GraphTag::COMPONENT)) continue;

                auto* v = it_adj.get_tgt_node();
                
                if (!v->is_visited(Designar::GraphTag::COMPONENT)) 
                {
                    v->visit(Designar::GraphTag::COMPONENT);
                    cola.push_back(v);
                    
                    auto* v_sub = subgrafo.insert_node(v->get_info());
                    mapa_nodos[v] = v_sub;
                    subgrafo.insert_arc(u_sub, v_sub, arco->get_info());
                } 
                else if (mapa_nodos.find(v) != mapa_nodos.end()) 
                {
                     if (subgrafo.search_arc(u_sub, mapa_nodos[v]) == nullptr) 
                     {
                        subgrafo.insert_arc(u_sub, mapa_nodos[v], arco->get_info());
                     }
                }
            }
        }
        
        segmentos.append(std::move(subgrafo));
    }

    return segmentos;
}