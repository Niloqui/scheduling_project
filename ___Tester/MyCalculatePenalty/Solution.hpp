#ifndef Solution_hpp
#define Solution_hpp

#include "graphw.hpp"
#include <fstream>
#include <string>

class Solution{ // Verione ridotta della classe Solution originale
public:
	int* sol = NULL;
	int n = -1, tmax = -1, penalty = -1;

	Solution(int n, int tmax);

	int calculatePenalty(G::Graph g);
	//int calculatePenalty(int* indexvector, int nsub);
	// Tutte le funzioni calculatePenalty restituiscono -1 se la soluzione è infeasible
	// La penalità non viene divisa per il numero di studenti

	// ~Solution() { delete[] sol; delete[] indexexams; }

	// Altro
	int distance(int i, int j);
	static int distance(int *sol, int i, int j);
};

#endif
