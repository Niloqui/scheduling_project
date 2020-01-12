#ifndef graphw_hpp
#define graphw_hpp

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>

using namespace boost;

//Classe wrapper per il grafo
class G {
public:
    typedef property<edge_weight_t, int> EdgeWeightProperty;
    typedef property<vertex_color_t, int> VertexColorProperty;
    typedef adjacency_list<listS, vecS, undirectedS, VertexColorProperty, EdgeWeightProperty> Graph;
    typedef Graph::edge_descriptor Edge;
    typedef Graph::vertex_descriptor Vertex;
    typedef graph_traits<G::Graph>::adjacency_iterator AdjacencyIterator;

};

#endif /* graphw_hpp */
