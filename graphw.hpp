#ifndef graphw_hpp
#define graphw_hpp

#include <stdio.h>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

//Classe wrapper per il grafo
class G{
    
public:
        typedef property<edge_weight_t, int> EdgeWeightProperty;
        typedef property<vertex_color_t,int> VertexColorProperty;
        typedef adjacency_list<listS, vecS,undirectedS,VertexColorProperty,EdgeWeightProperty> Graph;
        typedef Graph::edge_descriptor Edge;
        typedef Graph::vertex_descriptor Vertex;
    
};

#endif /* graphw_hpp */
