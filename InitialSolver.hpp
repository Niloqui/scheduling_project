#ifndef InitialSolver_hpp
#define InitialSolver_hpp

#include <iostream>
#include "graphw.hpp"
#include "Solution.hpp"
#include <boost/graph/adjacency_list.hpp>

class InitialSolver{
private:
	static bool recursiveFirstSolutionRec(G::Graph g, int *res, int* indexvector, int n, int tmax, int i);

public:
	static void greedy(G::Graph g, int* res, int* indexvector, int nsub, int tmax);
	// Non sermpre restituisce una soluzione feasible

	static int *recursiveFirstSolution(G::Graph g, int* indexvector, int nsub, int tmax);
	// Restituisce il vettore con i time slot per esame
	// Questa funzione è molto molto molto lenta, però funziona

	static std::pair<int, bool> firstPossiblePosition(G::Graph g, int* res, int* indexvector, int n, int tmax);
	// La funzione restituisce una coppia di valori
	// Il numero intero indica il timeslot massimo usato. Al massimo, si useranno n timeslot
	// La variabile booleana indica se la soluzione è infeasible (cioè se vengono usati al massimo tmax timeslot o meno)
	// true => la soluzione è feasible
	// false => la soluzione è infeasible

	static std::pair<int, bool> squeakyWheel(G::Graph g, int* res, int* indexvector, int n, int tmax);
	static std::pair<int, bool> squeakyWheel(G::Graph g, Solution* sol);
	// La prima funzione modifica i vettori passati come res e indexvector
	// La seconda funzione modifica il vettore sol e il vettore indexexams della classe Solution
	// Se indexexams == NULL, allora viene creato il vettore indexexams
	

};

#endif