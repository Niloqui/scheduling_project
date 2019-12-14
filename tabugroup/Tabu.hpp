#ifndef Tabu_hpp
#define Tabu_hpp

#include <stdio.h>
#include "graphw.hpp"
#include "Solution.hpp"

class Tabu{
public:
    Tabu(int);
    //Risolutore First improvement
    void solveFI(G::Graph& g, Solution& s);

private:
    int iterations;
    
};


#endif /* Tabu_hpp */
