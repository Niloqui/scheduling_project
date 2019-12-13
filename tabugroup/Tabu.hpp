#ifndef Tabu_hpp
#define Tabu_hpp

#include <stdio.h>
#include "Solution.hpp"
#include "graphw.hpp"
#include <boost/graph/adjacency_list.hpp>

class Tabu{
    
    public :
        //Colora il grafo con il vettore soluzione
        void colorGraph(G::Graph& g,Solution& s);
        
        //Operazione duale al colorGraph, questa volta
        //il grafo stampa sulla soluzione
        void setSolution(G::Graph& g, Solution& s);
        void simpleKempe(G::Graph& g ,G::Vertex v, int color);
        Tabu(); //costruttore vuoto
        ~Tabu();
    
        //Diventeranno private ma adesso sono pubbliche per testarle
    //Calcola la penalità associata nel far diventare v di colore = color
    //attraverso il kempe swap, quindi prende in considerazione la trasformazione
    //dei vicini di colore = color
    int nodeMovePenalty(G::Graph& g, G::Vertex v, int color);
        

    
    //private: //Da mettere tabu list, da decidere struttura dati
    
};

#endif /* Tabu_hpp */
