#include "InitialSolver.hpp"

#include <iostream>
#include "graphw.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <ctime>

std::pair<int, bool> InitialSolver::firstPossiblePosition(G::Graph *g, int* res, int* indexexams, int n, int tmax) {
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
			e = edge(indexexams[i], indexexams[j], *g);
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

/*
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
*/

std::pair<int, bool> InitialSolver::squeakyWheel(Solution* sol) {
	int* blame = new int[sol->n];
	int* indexexams = new int[sol->n];
	int* soltemp = new int[sol->n];
	int i, alpha = 1, beta = sol->tmax / 2;
	std::pair<int, bool> temp;
	temp.second = false;

	srand(time(NULL));
	int* weight = new int[sol->n];
	for (int i = 0; i < sol->n; i++) {
		blame[i] = 0;
		indexexams[i] = i;
		weight[i] = rand();
	}
	mergeSort(indexexams, weight, sol->n);
	delete[] weight;

	while (!temp.second) {
		temp = InitialSolver::firstPossiblePosition(sol->g, soltemp, indexexams, sol->n, sol->tmax);

		if (!temp.second) {
			for (i = 0; i < sol->n; i++) {
				if (sol->sol[i] > sol->tmax * alpha)
					blame[i] += beta + soltemp[i];
				else
					blame[i] += soltemp[i]; // blame[i] += i;
			}

			mergeSort(indexexams, blame, sol->n);
			reverseVector(indexexams, sol->n);
			reverseVector(blame, sol->n);
		}
	}

	for (i = 0; i < sol->n; i++) {
		sol->sol[indexexams[i]] = soltemp[i];
	}

	delete[] soltemp;
	delete[] indexexams;
	delete[] blame;
	return temp;
}


