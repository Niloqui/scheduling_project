#include "NiloSearch.hpp"

#include "ColorShift.hpp"
#include <ctime>
#include <iostream>

void NiloSearch::search(G::Graph* g, Solution* best, clock_t tlim, Solution* mothersolution) {
	tlim = (((tlim<<1) - 1) * CLOCKS_PER_SEC)>>1;
	// Conversione di tlim da secondi a clock_per_sec
	// Il meno 1 serve ad avere mezzo secondo di tempo finire l'esecuzione del programma

	std::pair<int*, int> pippo;
	pippo.second = -1;
	pippo.first = NULL;
	int shiftnum, i;

	srand(time(NULL) + clock());

	Solution* temp = new Solution(best);
	Solution* sol = new Solution(best);
	sol->mat = temp->mat = best->buildMatrix(g);
	sol->penalty = temp->penalty = best->calculatePenalty();

	// used_time = clock() - start;
	while (isThereTime(tlim)) {
		// Ricerca della soluzione migliore nel vicinato
		// LS(sol, temp, tlim, 2);
		LS(sol, temp, tlim, 1);

		if (sol->penalty < best->penalty) {
			best->setSolution(sol, false);
			best->penalty = sol->penalty;

			mothersolution->setSolutionAndPrint(g, sol, false);
		}

		// Perturbazione
		if (isThereTime(tlim)) {
			temp->mat = sol->mat;
			temp->penalty = sol->penalty;

			shiftnum = rand() % 3 + 1;
			// shiftnum = 2;

			for (i = 0; i < shiftnum; i++) {
				// pippo.second = sol->tmax * 2 / 5;
				pippo.second = 3 + rand() % 4;
				// pippo.second = 4;

				ColorShift::colorShift(temp, pippo, 2);
			}

			temp->calculatePenalty();
			sol->setSolution(temp, false);
			sol->penalty = temp->penalty;

			if (sol->penalty < best->penalty) {
				best->setSolution(sol, false);
				best->penalty = sol->penalty;

				mothersolution->setSolutionAndPrint(g, sol, false);
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
	int i, j, k = 0, h;
	int shiftI, shiftJ;

	while (ricerca && isThereTime(tlim)) {
		first_improvement = false;
		shiftI = rand() % sol->tmax;

		for (i = 1; i <= sol->tmax && isThereTime(tlim) && !first_improvement; i++) {
			shiftJ = rand() % sol->tmax;

			for (j = i + 1; j <= sol->tmax && isThereTime(tlim) && !first_improvement; j++) {
				cols.first[0] = (shiftI + i) % sol->tmax + 1;
				cols.first[1] = (shiftJ + j) % sol->tmax + 1;
				ColorShift::colorShift(temp, cols, 2);

				temp->calculatePenalty();
				if (temp->penalty < sol->penalty) {
					first_improvement = true;
					sol->setSolution(temp, false);
					sol->penalty = temp->penalty;
				}
				else {
					for (h = 0; h < sol->n; h++)
						temp->sol[h] = sol->sol[h];
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






/*
void NiloSearch::search(G::Graph* g, Solution* best, clock_t tlim, Solution* mothersolution, Solution* best_sub_problem) {
	tlim = (((tlim << 1) - 1) * CLOCKS_PER_SEC) >> 1;
	// Conversione di tlim da secondi a clock_per_sec
	// Il meno 1 serve ad avere mezzo secondo di tempo finire l'esecuzione del programma

	std::pair<int*, int> pippo;
	pippo.second = -1;
	pippo.first = NULL;
	int shiftnum, i;

	srand(time(NULL) + clock());

	Solution* temp = new Solution(best);
	Solution* sol = new Solution(best);
	sol->mat = temp->mat = best->buildMatrix(g);
	sol->penalty = temp->penalty = best->calculatePenalty();

	// used_time = clock() - start;
	while (isThereTime(tlim)) {
		// Ricerca della soluzione migliore nel vicinato
		// LS(sol, temp, tlim, 2);
		LS(sol, temp, tlim, 1);

		if (sol->penalty < best->penalty) {
			best->setSolution(sol, false);
			best->penalty = sol->penalty;

			if (best->penalty < best_sub_problem->penalty) {
				best_sub_problem->setSolution(best, false);
				best_sub_problem->penalty = best->penalty;

				mothersolution->setSolutionAndPrint(g, sol, true);
			}
		}

		// Perturbazione
		if (isThereTime(tlim)) {
			temp->mat = sol->mat;
			temp->penalty = sol->penalty;

			shiftnum = rand() % 3 + 1;
			// shiftnum = 2;

			for (i = 0; i < shiftnum; i++) {
				// pippo.second = sol->tmax * 2 / 5;
				pippo.second = 3 + rand() % 4;
				// pippo.second = 4;

				ColorShift::colorShift(temp, pippo, 2);
			}

			temp->calculatePenalty();
			sol->setSolution(temp, false);
			sol->penalty = temp->penalty;

			if (sol->penalty < best->penalty) {
				best->setSolution(sol, false);
				best->penalty = sol->penalty;

				if (best->penalty < best_sub_problem->penalty) {
					best_sub_problem->setSolution(best, false);
					best_sub_problem->penalty = best->penalty;

					mothersolution->setSolutionAndPrint(g, sol, true);
				}
			}
		}
	}

	delete[] temp->indexexams;
	delete[] temp->sol;
	delete temp;
}
*/