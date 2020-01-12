#ifndef Kempe_hpp
#define Kempe_hpp

#include "../Solution.hpp"
    
//Colora il grafo con il vettore soluzione
void colorGraph(G::Graph& g,Solution& s);

//Operazione duale al colorGraph, questa volta
//il grafo stampa sulla soluzione
void setSolution(G::Graph& g, Solution& s);


//Cambia il colore del vertice v dal suo colore originale al colore
//che viene fornito come paramentro, i vertici ad esso adiacenti che hanno
//il colore fornito verrano camibati al colore del vertice v
//e cosi via ricorsivamente lungo tutta la catena, effettuando un kempe swap
//rimanendo comunque nella feasibility, se la soluzione fornita inizialmente
//era già feasible
void simpleKempe(G::Graph& g, G::Vertex v, int color, unordered_set<long int>& visitedNodes);

//Trasforma il colore del vertice v nel colore parametro
//Questo cambio viene effettuato lungo la catena kempe
//Ovvero i nodi che sono adiacenti a v e sono di colore color
//diventano adesso del colore originale di v. Questo cambio
//viene propagato lungo tutta la catena in maniera da rimanere nella feasibility.
void simpleKempeWrapper(G::Graph& g ,G::Vertex v, int color);


//Calcola la penalità associata nel far diventare v di colore = color
//attraverso il kempe swap, quindi prende in considerazione la trasformazione
//dei vicini di colore = color
int nodeMovePenalty(G::Graph& g, G::Vertex v, int color);

//Usa nodeMovePenalty per calcolare il costo di far diventare il vertice
//v del colore color, e propaga ricorsivamente il calcola a tutti
//i nodi associati alla catena che parte da v e va verso i colori
//adiacenti del colore fornito come parametro
int kempeMovePenaltyWrapper(G::Graph& g, G::Vertex v, int color);

//Calcola la penalità associata al nodo corrente
int nodeCurrentPenalty(G::Graph& g, G::Vertex v);


#endif /* Kempe */
