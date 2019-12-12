#ifndef Tabu_hpp
#define Tabu_hpp

#include <stdio.h>
#include "Solution.hpp"
#include "graphw.hpp"
#include <boost/graph/adjacency_list.hpp>

class Tabu{
    
    public :
        void colorGraph(G::Graph& g,Solution& s);
        Tabu(); //costruttore vuoto
        ~Tabu();

    
    //private: //Da mettere tabu list, da decidere struttura dati
    
};

#endif /* Tabu_hpp */
