#include "NiloSearch.hpp"

#include "ColorShift.hpp"
#include <ctime>
#include <iostream>

void NiloSearch::search(G::Graph* g, Solution* sol, clock_t tlim) {
	tlim = (tlim - 1) * CLOCKS_PER_SEC;
	// Conversione di tlim da secondi a clock_per_sec
	// Il meno 1 serve ad avere un secondo di tempo finire l'esecuzione del programma

	std::pair<int*, int> pippo;
	pippo.second = -1;
	pippo.first = NULL;
	int shiftnum, i;

	srand(time(NULL) + clock());

	Solution* temp = new Solution(sol);
	temp->mat = sol->buildMatrix(g);
	temp->penalty = sol->calculatePenalty();

	// used_time = clock() - start;
	while (isThereTime(tlim)) {
		// Ricerca della soluzione migliore nel vicinato;
		LS(sol, temp, tlim, 2);

		// Perturbazione
		if (isThereTime(tlim)) {
			// shiftnum = rand() % 3 + 1;
			shiftnum = 2;
			// pippo.second = sol->tmax * 2 / 5;
			pippo.second = 4;
			for (i = 0; i < shiftnum; i++) {
				ColorShift::colorShift(temp, pippo, false);
			}
		}
	}

	delete[] temp->indexexams;
	delete[] temp->sol;
	delete temp;
}

void NiloSearch::LS(Solution* sol, Solution* temp, clock_t tlim, int attempts) {
	bool ricerca = true, first_improvement;
	std::pair<int*, int> cols;
	cols.second = 2;
	cols.first = new int[2];
	int i, j, k = 0;
	int shiftI, shiftJ; // da implementare

	while (ricerca && isThereTime(tlim)) {
		first_improvement = false;
		shiftI = rand() % sol->tmax;

		for (i = 1; i <= sol->tmax && isThereTime(tlim) && !first_improvement; i++) {
			shiftJ = rand() % sol->tmax;

			for (j = i + 1; j <= sol->tmax && isThereTime(tlim) && !first_improvement; j++) {
				cols.first[0] = (shiftI + i) % sol->tmax + 1;
				cols.first[1] = (shiftJ + j) % sol->tmax + 1;
				ColorShift::colorShift(temp, cols, true);

				if (temp->calculatePenalty() < sol->penalty) {
					first_improvement = true;
					sol->setSolution(temp, false);
					sol->calculatePenalty();
				}
			}
		}

		if (first_improvement) {
			k = 0;
		}
		else {
			// L'algoritmo proverà più volte la stessa coppia di colori
			// In questo modo è possibile vedere più catene appartenenti alla stessa coppia di colori
			k++;
			if (k >= attempts)
				ricerca = false;
		}
		
	}

	delete[] cols.first;
}

bool NiloSearch::isThereTime(clock_t tlim) {
	return clock() < tlim;
}

