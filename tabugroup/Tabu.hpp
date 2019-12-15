#ifndef Tabu_hpp
#define Tabu_hpp

#include <stdio.h>
#include "graphw.hpp"
#include "Solution.hpp"

class Tabu{
public:
    Tabu(int,int);
    //Risolutore First improvement
    void solveFI(G::Graph& g, Solution& s);
    void steepestDescent(G::Graph& g, Solution& s);
    
    //Q è il numero di vertici che verrano scelti per venire perturbati
    //Vengono ordinati per penalità, i q più penalizzati vengono perturbati
    //Eta è il numero di perturbazioni che subiscono in totale
    void perturbate(G::Graph& g, int q,int eta);

private:
    int iterations;
    int tmax;
    
};


#endif /* Tabu_hpp */
