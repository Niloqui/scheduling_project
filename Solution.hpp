#ifndef Solution_hpp
#define Solution_hpp

#include "graphw.hpp"

class Solution{
public:
	int* sol = NULL;
	/* L'indice di un elemento del vettore sol indica l'esame
	 * Gli esami sono sfasati di uno rispetto all'indice nel vettore
	 * Il valore di un elemento del vettore sol indica il suo time slot
	 * ex. sol[4] = 7; L'esame 5 (4 = 5 - 1) si trova nel time slot 7
	 * Il reader diminuisce l'esame di 1, quindi non è necessario eseguire la sottrazione scritta sopra per accedere al vettore
	 */
	int n = -1, tmax = -1, penalty = -1;
	// n = lunghezza del vettore
	std::string filename; // File dove stampare il risultato. Utile solo per mothersolution


	Solution(int n, int tmax); // Costruttore
	Solution(Solution *sol); // Costruttore che copia da un'altra soluzione

	void setSolution(Solution* newsol);
	// Si assume che newsol abbia la stessa lunghezza di this->sol

	int calculatePenalty(G::Graph& g);
	// Restituiscono -1 se la soluzione è infeasible
	// La penalità non viene divisa per il numero di studenti

	int distance(int i, int j);

	std::string checkSetPrintSolution(G::Graph* g, Solution* sol);
	// Questa funzione serve per scrivere in mothersolution
	// Inoltre esegue il controllo per capire se sol è migliore di this
	// Esporta la soluzione su file e ritorna l'intero output che viene stampato a video
};
	
	

#endif