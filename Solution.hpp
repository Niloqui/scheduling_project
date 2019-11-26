#ifndef Solution_hpp
#define Solution_hpp

#include "graphw.hpp"
#include <iostream>
#include <string>
#include <string.h>

class Solution{
private:
	int* sol;
	int n, tmax;
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