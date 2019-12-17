#ifndef Kempe_hpp
#define Kempe_hpp

#include <stdio.h>
#include "Solution.hpp"
#include "graphw.hpp"
#include <boost/graph/adjacency_list.hpp>

    
//Colora il grafo con il vettore soluzione
void colorGraph(G::Graph& g,Solution& s);

//Operazione duale al colorGraph, questa volta
//il grafo stampa sulla soluzione
void setSolution(G::Graph& g, Solution& s);

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

//Calcola la penalità associata nel far diventare il vertice v di colore
//pari al parametro color, questo costo viene propagato lungo la catena kempe
//una penalità negativa indica che quella mossa diminuirebbe il valore della
//funzione obiettivo
int kempeMovePenaltyWrapper(G::Graph& g, G::Vertex v, int color);

//Calcola la penalità associata al nodo corrente
int nodeCurrentPenalty(G::Graph& g, G::Vertex v);



#endif /* Kempe */
