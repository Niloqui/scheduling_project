#ifndef Tabu_hpp
#define Tabu_hpp

#include <stdio.h>
#include "graphw.hpp"
#include "Solution.hpp"

class Tabu{
public:
    Tabu(int);
    
private:
    int teta; //Lunghezza della tabu list
    //Lista delle mosse proibite, il primo intero è l'id del vettore
    //Il secondo elemento è il colore al quale non si può muovere
    std::vector<std::pair<long int,int>> tabuList;
    
    //Cerca la lista Tabu per vedere se una mossa è proibita
    bool isTabu(long int id, int color);
    bool isTabu(G::Graph& g,G::Vertex v, int color);
    
    //Inserire mossa nella tabu list
    void insertMove(G::Vertex,int color);
    void insertMove(long int id,int color);
    
    //
    void tabuIteratedLocalSearch(G::Graph& g, Solution& s,int tollerance,clock_t start,int tlim);

    
};


#endif /* Tabu_hpp */
