#include <iostream>
#include "Tabu.hpp"
#include "Solution.hpp"
#include "graphw.hpp"
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

Tabu::Tabu(){}
Tabu::~Tabu(){/*delete something*/}

void Tabu::colorGraph(G::Graph& g,Solution& s){
    long int id;
    int color;
        
    //Itero sui vertici del grafo in ordine di nome
    //E si colorano i grafi con la soluzione iniziale
    
    //Inoltre vertices ritorna un iterator, se ci serve un descriptor
    //basta dereferenziare
    
    G::Graph::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(g); v != vend; ++v) {
        id = get(vertex_index_t(),g,*v);
        color = s.sol[id];
        put(vertex_color_t(),g,*v,s.sol[id]);
    }
}
