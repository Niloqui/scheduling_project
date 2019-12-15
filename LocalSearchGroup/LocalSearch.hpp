#ifndef LocalSearch_hpp
#define LocalSearch_hpp
#include "Solution.hpp"
#include "graphw.hpp"

#include <stdio.h>

//Risolutore First improvement
void solveFI(G::Graph& g, Solution& s,int iterations);
void steepestDescent(G::Graph& g, Solution& s, int iterations);

//Q è il numero di vertici che verrano scelti per venire perturbati
//Vengono ordinati per penalità, i q più penalizzati vengono perturbati
//Eta è il numero di perturbazioni che subiscono in totale
void perturbate(G::Graph& g, int q,int eta, int tmax);

//tollerance specifica il numero di iterazioni durante le quali si tollera
//il non miglioramento della soluzione
//se non c'è un miglioramento dopo un numero tollerance di steepestDescent allora
//si procede nella perturbation
void iteratedLocalSearch(G::Graph& g, Solution& s,int iterations,int tollerance);


#endif /* LocalSearch_hpp */
