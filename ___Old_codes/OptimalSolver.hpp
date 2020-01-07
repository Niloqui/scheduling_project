#ifndef OptimalSolver_hpp
#define OptimalSolver_hpp

#include "Solution.hpp"
#include "graphw.hpp"

#define MAX_O 10000000
// MAX_O serve come massimo numero di operazioni da eseguire in caso di risolutore ottimale

class OptimalSolver{
	// Se un sottoproblema è piccolo, lo si può risolvere con un metodo esatto
	// Il metodo esatto è O(tmax^n)

private:
	// static void solveSmallProblemRec(G::Graph g, Solution* sol, Solution* temp, int exam);
	static void solveSmallProblemRec(Solution* sol, Solution* temp, int exam);

public:
	static bool problemIsReallySmall(Solution* sol);
	static bool problemIsReallySmall(int n, int tmax);
	// Gli esami si possono mettere ad intervalli regolari senza avere penalità
		// es: n = 3, tmax = 15
		// 1,_,_,_,_,_,2,_,_,_,_,_,3,_,_

	static bool problemIsSmall(Solution* sol, int maxO);
	// Il metodo esatto è O(tmax^n)

	static void solveReallySmallProblem(Solution *sol);
	static void solveSmallProblem(G::Graph *g, Solution* sol);
	// Entrambi i solver assumono che il problema sia effettivamente piccolo (o molto piccolo). Non eseguono il controllo





};


#endif