#ifndef IlsOptimizer_hpp
#define IlsOptimizer_hpp

#include <stdio.h>
#include "LsSolver.hpp"
#include "Perturbator.hpp"
#include "Solution.hpp"

class ILS {
private:
    // valutare se i puntatori sono necessari
    
    LS *localSearch;
    Perturbator *perturbator;
    Solution *currentSol;
    

public:
    
    ILS(G::Graph graph, int maxEvals=-1, Solution *solution = nullptr){
        this->currentSol = solution;
        this->localSearch = new LS(graph, maxEvals, solution);
        this->perturbator = new Perturbator();
    }
    
    
    //Solution *getInitialSol();
    
    void doLocalSearch();
    
    void updateCurrentSol();
    
    void doPerturbation();
    
    void acceptanceTest();
    
    void stoppingCriterion();
    
    // vari metodi per gestione metaeuristica:
    
    // crea soluzione iniziale
    // fai una ricerca locale su questa soluzione iniziale (in un determinato vicinato)
    // aggiorna la soluzione corrente con questa nuova soluzione
    
    // pertuba la soluzione corrente andando a prendere una soluzione casuale appartenente a un 
    // più grande di quello usato per la ricerca locale
    // test di accettazione
    // Criterio di stop
    
    // forse la ricerca locale ha bisogno di un suo criterio di stop per non iterare troppe volte


    ~ILS() { delete this->localSearch; delete this->perturbator; }
    
};



#endif /* IlsOptimizer_hpp */
