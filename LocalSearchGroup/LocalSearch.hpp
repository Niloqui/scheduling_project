#ifndef LocalSearch_hpp
#define LocalSearch_hpp
#include "Solution.hpp"
#include "graphw.hpp"
#include <ctime>

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
void iteratedLocalSearch(G::Graph& g, Solution& s,int tollerance,clock_t start,int tlim);

//Fa lo swap dei colori 1 e 2, ma non li segna sulla soluzione
void swapColors(G::Graph& g,int color1, int color2);

//Calcola quanto potrebbe cambiare la penalità uno swap fra colore1 e colore2
int swapColorsPenalty(G::Graph& g,Solution& s ,int color1, int color2);

/*Improvement basato sullo swap di colori e non di catene**/
//Potrebbe essere utile per una prima esplorazione dello spazio
//Ritorna true se c'è stato un miglioramento
bool firstColorImprovement(G::Graph& g,Solution& s);

//Effettua un ciclo di first improvement finchè non ci sono
//Più miglioramenti
void firstColorImprovementLoop(G::Graph& g,Solution &s,int iterations);


#endif /* LocalSearch_hpp */
