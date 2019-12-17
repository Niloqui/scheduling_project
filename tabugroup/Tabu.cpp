#include "Tabu.hpp"
#include "../kempegroup/Kempe.hpp"
#include <algorithm>
#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


using namespace std;

Tabu::Tabu(int teta):teta(teta){}

//Cerca la lista Tabu per vedere se una mossa è proibita
bool Tabu::isTabu(long int id, int color){
    for (auto i = this->tabuList.begin(); i != this->tabuList.end(); ++i){
        if(id == (*i).first && color == (*i).second)
            return true;
    }
    return false;
}

bool Tabu::isTabu(G::Graph& g, G::Vertex v, int color){
    return isTabu(get(vertex_index_t(),g,v), color);
}

//Inserire mossa nella tabu list
//Elimina automaticamente mosse che ingrandiscano la lista oltre teta
void insertMove(G::Vertex,int color){
    
}


void insertMove(long int id,int color);

//
void tabuIteratedLocalSearch(G::Graph& g, Solution& s,int tollerance,clock_t start,int tlim);
