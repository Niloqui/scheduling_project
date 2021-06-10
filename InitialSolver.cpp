#include "InitialSolver.hpp"

#include <ctime>
#include "Utility.hpp"

std::pair<int, bool> InitialSolver::firstPossiblePosition(G::Graph *g, int* res, int* indexexams, int n, int tmax) {
	int i, j;
	bool* available = new bool[n];
	std::pair<G::Edge, bool> e;
	std::pair<int, bool> max;
	max.first = 0;
	max.second = true;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			available[j] = true;

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

	delete[] available;
	max.first++;
	return max;
}
int InitialSolver::squeakyWheel(G::Graph* g, Solution* sol) {
	int* blame = new int[sol->n];
	int* indexexams = new int[sol->n];
	int* soltemp = new int[sol->n];
	int i, overblame = sol->tmax / 2;
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
		temp = InitialSolver::firstPossiblePosition(g, soltemp, indexexams, sol->n, sol->tmax);

		if (!temp.second) {
			for (i = 0; i < sol->n; i++) {
				blame[i] += soltemp[i]; // blame[i] += i;

				if (sol->sol[i] > sol->tmax)
					blame[i] += overblame;
			}

			mergeSort(indexexams, blame, sol->n);
			reverseVector(indexexams, sol->n);
			reverseVector(blame, sol->n);
		}
	}

	for (i = 0; i < sol->n; i++)
		sol->sol[indexexams[i]] = soltemp[i];

	delete[] soltemp;
	delete[] indexexams;
	delete[] blame;
	return temp.first;
}


