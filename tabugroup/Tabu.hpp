#ifndef Tabu_hpp
#define Tabu_hpp

#include <stdio.h>
#include "graphw.hpp"
#include "Solution.hpp"

class Tabu{
public:
    Tabu(int); //Accetta teta, ovvero la grandezza della tabu list
    void tabuIteratedLocalSearch(G::Graph& g, Solution& s,int tollerance,clock_t start,int tlim);
    //Max non improving iterations da il numero di mosse massime senza miglioramenti
    //Che sono permesse prima che il tabu search venga perturbato e subentri la
    //fase di iterated local search
    // ritorna True se il vicinato ha trovato un ottimo globale
    bool tabuSearch(G::Graph& g, Solution& s,int maxNonImprovingIterations, int bestGlobalPenalty,clock_t start,int tlim);
    
private:
    int teta; //Lunghezza della tabu list
    //Lista delle mosse proibite, il primo intero è l'id del vettore
    //Il secondo elemento è il colore al quale non si può muovere
    std::deque<std::pair<long int,int>> tabuList;
    
    //Cerca la lista Tabu per vedere se una mossa è proibita
    bool isTabu(long int id, int color);
    bool isTabu(G::Graph& g,G::Vertex v, int color);
    
    //Inserire mossa nella tabu list
    void insertMove(G::Vertex v, G::Graph& g,int color);
    void insertMove(long int id,int color);
    
    //
    void tabuSimpleKempeWrapper(G::Graph& g ,G::Vertex v, int color);
    void tabuSimpleKempe(G::Graph& g ,G::Vertex v, int color,std::unordered_set<long int>& visitedNodes);
    void resetTabuList();
    double mu; //moltiplicatore per la profondità della tabu list



    
};


#endif /* Tabu_hpp */
