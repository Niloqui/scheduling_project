#include "InitialSolver.hpp"

#include <iostream>
#include "graphw.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
#include <boost/graph/adjacency_list.hpp>

void InitialSolver::greedy(G::Graph g, int* res, int* indexvector, int nsub, int tmax) {
	int i, j, newpen, minpen, jmin;

	for (i = 0; i < nsub; i++) {
		minpen = INT32_MAX;
		jmin = -1;

		for (j = 1; j <= tmax; j++) {
			res[indexvector[i]] = j;
			newpen = Solution::calculatePenalty(g, res, indexvector, i + 1, tmax);
			if ((newpen >= 0) && (newpen < minpen)) {
				minpen = newpen;
				jmin = j;
			}
		}

		res[indexvector[i]] = jmin;
	}
}

bool InitialSolver::recursiveFirstSolutionRec(G::Graph g, int* res, int* indexvector, int nsub, int tmax, int i) {
	int j, k;
	bool* available = new bool[tmax];
	std::pair<G::Edge, bool> e;

	if (i == nsub)
		return true;

	for (j = 0; j < tmax; j++)
		available[j] = true;

	for (k = 0; k < i; k++) {
		e = edge(indexvector[i], indexvector[k], g);
		if (e.second)
			available[res[k] - 1] = false;
	}

	for (j = 0; j < tmax; j++) {
		if (available[j]){
			res[i] = j + 1;
			if (InitialSolver::recursiveFirstSolutionRec(g, res, indexvector, nsub, tmax, i + 1)) {
				// Soluzione trovata
				delete[] available;
				return true;
			}
		}
	}

	delete[] available;
	return false;
}

int* InitialSolver::recursiveFirstSolution(G::Graph g, int* indexvector, int nsub, int tmax) {
	int* res = new int[nsub];

	for (int i = 0; i < nsub; i++)
		res[i] = -1;

	if (!InitialSolver::recursiveFirstSolutionRec(g, res, indexvector, nsub, tmax, 0)) {
		delete[] res;
		return NULL;
	}
	return res;
}

std::pair<int, bool> InitialSolver::firstPossiblePosition(G::Graph g, int* res, int* indexvector, int n, int tmax) {
	int i, j;
	bool* available = new bool[n];
	std::pair<G::Edge, bool> e;
	std::pair<int, bool> max;
	max.first = 0;
	max.second = true;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			available[j] = true;
		}

		for (j = 0; j < i; j++) {
			e = edge(indexvector[i], indexvector[j], g);
			if (e.second) {
				available[res[j] - 1] = false;
			}
		}

		for (j = 0; j < n; j++) {
			if (available[j]) {
				res[i] = j + 1;
				
				if (j > max.first)
					max.first = j;
				if (j >= tmax)
					max.second = false;

				break;
			}
		}
	}

	max.first++;
	return max;
}

std::pair<int, bool> InitialSolver::squeakyWheel(G::Graph g, int* res, int* indexvector, int n, int tmax) {
	int* blame = new int[n];
	int i, alpha = 1, beta = tmax / 2;
	std::pair<int, bool> temp;
	temp.second = false;

	for (i = 0; i < n; i++)
		blame[i] = 0;

	while (!temp.second) {
		temp = InitialSolver::firstPossiblePosition(g, res, indexvector, n, tmax);

		if (!temp.second) {
			for (i = 0; i < n; i++) {
				if (res[i] > tmax* alpha)
					blame[i] += beta + res[i];
				else
					blame[i] += res[i]; // blame[i] += i;
			}

			mergeSort(indexvector, blame, n);
			reverseVector(indexvector, n);
			reverseVector(blame, n);
		}
	}

	delete[] blame;
	return temp;
}

std::pair<int, bool> InitialSolver::squeakyWheel(G::Graph g, Solution* sol) {
	int* blame = new int[sol->n];
	int* indexvector = NULL;
	int i, alpha = 1, beta = sol->tmax / 2;
	std::pair<int, bool> temp;
	temp.second = false;

	for (i = 0; i < sol->n; i++)
		blame[i] = 0;

	if (sol->indexexams == NULL)
		indexvector = new int[sol->n];
	else
		indexvector = sol->indexexams;

	while (!temp.second) {
		temp = InitialSolver::firstPossiblePosition(g, sol->sol, indexvector, sol->n, sol->tmax);

		if (!temp.second) {
			for (i = 0; i < sol->n; i++) {
				if (sol->sol[i] > sol->tmax* alpha)
					blame[i] += beta + sol->sol[i];
				else
					blame[i] += sol->sol[i]; // blame[i] += i;
			}

			mergeSort(indexvector, blame, sol->n);
			reverseVector(indexvector, sol->n);
			reverseVector(blame, sol->n);
		}
	}

	if (indexvector != sol->indexexams)
		delete[] indexvector;
	delete[] blame;
	return temp;
}


