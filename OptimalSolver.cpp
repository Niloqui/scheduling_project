#include "OptimalSolver.hpp"

#include "Solution.hpp"
#include "Utility.hpp"
#include <math.h>

bool OptimalSolver::problemIsReallySmall(Solution* sol) {
	if ( 6*(sol->n - 1) <= (sol->tmax - 1)  ) // sol->n < ((sol->tmax - 1)/6 + 1)
		// Gli esami si possono mettere ad intervalli regolari senza avere penalità
		// es: n = 3, tmax = 15
		// 1,_,_,_,_,_,2,_,_,_,_,_,3,_,_
		return true;
	return false;
}

bool OptimalSolver::problemIsReallySmall(int n, int tmax) {
	if (6 * (n - 1) <= (tmax - 1))
		return true;
	return false;
}

bool OptimalSolver::problemIsSmall(Solution *sol, int maxO) {
	if (sol->n >= 20)
		return false;

	// Il metodo esatto è O(tmax^n)
	double Ogrande = pow(sol->tmax, sol->n);
	
	if (Ogrande <= maxO)
		return true;
	return false;
}

void OptimalSolver::solveReallySmallProblem(G::Graph g, Solution* sol) {
	for (int i = 0; i < sol->n; i++)
		sol->sol[i] = i * 6 + 1;
	
	sol->optimum = true;
}

void OptimalSolver::solveSmallProblem(G::Graph g, Solution* sol) {
	


	// TO-DO: implementare risolutore ottimo


}

void OptimalSolver::solveSmallProblemRec(G::Graph g, Solution* best, Solution* sol, int i) {



	// TO-DO: implementare risolutore ottimo



}
