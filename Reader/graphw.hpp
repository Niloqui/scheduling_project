#ifndef graphw_hpp
#define graphw_hpp

#include <stdio.h>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

//Wrapper class for the actual graph
class G{
    
public:
        typedef property<edge_weight_t, int> EdgeWeightProperty;
        typedef adjacency_list<listS, vecS,undirectedS,no_property,EdgeWeightProperty> Graph;
        typedef Graph::edge_descriptor Edge;
    
};

#endif /* graphw_hpp */
