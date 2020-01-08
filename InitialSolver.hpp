#ifndef InitialSolver_hpp
#define InitialSolver_hpp

#include <iostream>
// #include <boost/graph/adjacency_list.hpp>
#include "graphw.hpp"
#include "Solution.hpp"

class InitialSolver{
public:
	static std::pair<int, bool> firstPossiblePosition(G::Graph *g, int* res, int* indexexams, int n, int tmax);
	// La funzione restituisce una coppia di valori
	// Il numero intero indica il timeslot massimo usato. Al massimo, si useranno n timeslot
	// La variabile booleana indica se la soluzione è infeasible (cioè se vengono usati al massimo tmax timeslot o meno)
	// true => la soluzione è feasible
	// false => la soluzione è infeasible

	static int squeakyWheel(G::Graph* g, Solution* sol);
	// La funzione restituisce il numero di timeslot utilizzati

};

#endif