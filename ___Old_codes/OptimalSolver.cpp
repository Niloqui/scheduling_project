#include "OptimalSolver.hpp"

#include "Solution.hpp"
#include "Utility.hpp"
#include "InitialSolver.hpp"
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

void OptimalSolver::solveReallySmallProblem(Solution* sol) {
	for (int i = 0; i < sol->n; i++)
		sol->sol[i] = i * 6 + 1;
	
	sol->optimum = true;
}

void OptimalSolver::solveSmallProblem(G::Graph *g, Solution* sol) {
	Solution *temp = new Solution(sol);
	temp->penalty = 0;

	InitialSolver::greedy(*g, sol);
	sol->penalty = sol->calculatePenalty(*g);
	if (sol->penalty == 0) {
		sol->optimum = true;
		return;
	}
	else if (sol->penalty < 0) {
		sol->penalty = INT32_MAX;
	}

	temp->mat = sol->buildMatrix(g);

	solveSmallProblemRec(sol, temp, 0);
	sol->optimum = true;

	delete[] temp->indexexams;
	delete[] temp->sol;
	delete temp;
}

void OptimalSolver::solveSmallProblemRec(Solution* sol, Solution* temp, int exam) {
	if (exam >= sol->n) {
		sol->setSolution(temp, false);
		sol->penalty = temp->penalty;
	}
	else {
		std::pair<G::Edge, bool> e;
		int i, t, addpen, dist;
		bool timeslot_disponibile;

		for (t = 1; t <= sol->tmax; t++) {
			addpen = 0;
			timeslot_disponibile = true;

			for (i = exam - 1; i >= 0 && timeslot_disponibile; i--) {
				if (sol->mat[i][exam] > 0) {
					if (temp->sol[i] == t) {
						timeslot_disponibile = false;
					}
					else {
						dist = absoluteDifference(temp->sol[i], t);
						if(dist <= 5)
							addpen += integerPower(2, 5 - dist) * sol->mat[i][exam];
					}
				}
			}

			temp->penalty += addpen;
			if (timeslot_disponibile && temp->penalty < sol->penalty) {
				temp->sol[exam] = t;
				solveSmallProblemRec(sol, temp, exam + 1);
			}
			temp->penalty -= addpen;
		}
	}
}

/*
void OptimalSolver::solveSmallProblemRec(G::Graph g, Solution* sol, Solution* temp, int exam) {
	if (exam >= sol->n) {
		sol->setSolution(temp, false);
		sol->penalty = temp->penalty;
	}
	else {
		std::pair<G::Edge, bool> e;
		int i, t, addpen, dist;
		bool timeslot_disponibile;

		for (t = 1; t <= sol->tmax; t++) {
			addpen = 0;
			timeslot_disponibile = true;

			for (i = exam - 1; i >= 0 && timeslot_disponibile; i--) {
				e = temp->getEdge(i, exam, g);

				if (e.second) {
					if (temp->sol[i] == t) {
						timeslot_disponibile = false;
					}
					else {
						dist = temp->sol[i] - t;
						dist = (dist < 0) ? (-dist) : dist;	
						addpen += integerPower(2, 5 - dist) * get(edge_weight_t(), g, e.first);
					}
				}
			}

			temp->penalty += addpen;
			if (timeslot_disponibile && temp->penalty < sol->penalty) {
				temp->sol[exam] = t;
				solveSmallProblemRec(g, sol, temp, exam + 1);
			}
			temp->penalty -= addpen;
		}
	}
}
*/

/*
void OptimalSolver::solveSmallProblemRec(G::Graph g, Solution* sol, Solution* temp, int exam) {
	if (exam >= sol->n) {
		bool newbest = false;
		int newpen = temp->calculatePenalty(g);

		if (newpen < (sol->penalty==-1?INT32_MAX:sol->penalty))
			newbest = true;

		if (newbest) {
			sol->setSolution(temp, false);
			sol->penalty = newpen;
		}
	}
	else {
		std::pair<G::Edge, bool> e;
		int i, t;
		bool timeslot_disponibile;
		for (t = 1; t <= sol->tmax; t++) {
			timeslot_disponibile = true;

			for (i = exam - 1; i >= 0; i--) {
				e = temp->getEdge(i, exam, g);

				if (e.second && temp->sol[i]==t)
					timeslot_disponibile = false;
			}

			if (timeslot_disponibile) {
				temp->sol[exam] = t;
				solveSmallProblemRec(g, sol, temp, exam + 1);
			}
		}
	}
}
*/
