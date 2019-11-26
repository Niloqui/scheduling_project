#ifndef Solution_hpp
#define Solution_hpp

#include "graphw.hpp"

class Solution{
private:
	int* sol;
	/* L'indice di un elemento del vettore sol indica l'esame
	 * Il valore di un elemento del vettore sol indica il suo time slot
	 * ex. sol[4] = 7; L'esame 4 si trova nel time slot 7
	 */
	int n, tmax; // n = lunghezza del vettore
	int distance(int i, int j);

public:
	Solution(int n, int tmax); // Costruttore
	Solution(int n, int tmax, int* newsol); // Costruttore con soluzione iniziale

	void setSolution(int* newsol);
	void setSolution(int* newsol, bool* mask);
	void setSolution(int* newsol, int* indexvector, int nsub);

	int calculatePenalty(G::Graph g);
	int calculatePenalty(G::Graph g, bool* mask);
	int calculatePenalty(G::Graph g, int* indexvector, int nsub);
	// tutte le funzioni calculatePenalty restituiscono -1 se la soluzione è unfeasible

	void printSolution(std::string filename); // Esporta la soluzione su file

};

#endif