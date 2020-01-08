#include "ColorShift.hpp"

#include <ctime>
#include <iostream>
#include <cstdlib>
#include "Utility.hpp"

void ColorShift::penaltyByColor(Solution* sol, int* penalty) {
	// penalty si assume inizializzato a 0
	// Si assume che la soluzione sia feasible
	int i, j, dist, pen;
	std::pair<G::Edge, bool> e;

	for (i = 0; i < sol->n; i++) {
		for (j = i + 1; j < sol->n; j++) {
			e = edge(i, j, *sol->g);
			if (e.second) {
				pen = boost::get(edge_weight_t(), *sol->g, e.first);
				if (pen > 0) {
					dist = sol->distance(i, j);
					if (dist <= 5) {
						pen = (1 << (5 - dist)) * pen;
						penalty[sol->sol[i] - 1] += pen;
						penalty[sol->sol[j] - 1] += pen;
					}
				}
			}
		}
	}
}

void ColorShift::penaltyByColor(Solution* sol, int* penalty_by_color, int* penalty_by_exam) {
	// penalty e penalty_by_exam si assumono inizializzati a 0
	// Si assume che la soluzione sia feasible
	int i, j, dist, pen;
	std::pair<G::Edge, bool> e;

	for (i = 0; i < sol->n; i++) {
		for (j = i + 1; j < sol->n; j++) {
			e = edge(i, j, *sol->g);
			if (e.second) {
				pen = boost::get(edge_weight_t(), *sol->g, e.first);
				if (pen > 0) {
					dist = sol->distance(i, j);
					if (dist <= 5) {
						pen = (1 << (5 - dist)) * pen;
						penalty_by_color[sol->sol[i] - 1] += pen;
						penalty_by_color[sol->sol[j] - 1] += pen;
						penalty_by_exam[i] += pen;
						penalty_by_exam[j] += pen;
					}
				}
			}
		}
	}

}

std::pair<int*, int> ColorShift::selectColors(Solution* sol, int ncols, int selection_mode, int* initial_node) {
	std::pair<int*, int> coso;
	if (ncols == -1)
		// coso.second = (int)( ((float)rand() / RAND_MAX) * ((sol->tmax * 5.0) / 7.0) ) + 2;
		coso.second = (int)(((float)rand() / RAND_MAX) * (sol->tmax * 0.4)) + 2;
	else
		coso.second = ncols;

	coso.first = new int[coso.second];
	// int* cols = new int[(int64_t)sol->tmax + 1];
	int* cols = new int[sol->tmax];
	int* colweight = new int[sol->tmax];
	int i;

	switch (selection_mode) {
	case 0: // I colori sono pesati a caso
		for (i = 0; i < sol->tmax; i++) {
			cols[i] = i + 1;
			colweight[i] = rand();
		}
		mergeSort(cols, colweight, sol->tmax);

		for (i = 0; i < coso.second; i++)
			coso.first[i] = cols[i];

		break;

	case 1:
		// I colori vengono scelti in base alla penalità
		// Vengono scelti i colori più pesanti
		for (i = 0; i < sol->tmax; i++) {
			cols[i] = i + 1;
			colweight[i] = 0;
		}

		penaltyByColor(sol, colweight);
		mergeSort(cols, colweight, sol->tmax);

		for (i = 0; i < coso.second; i++)
			coso.first[i] = cols[sol->tmax - 1 - i];

		break;

	case 2:
		// I colori vengono scelti in base alla penalità
		// Vengono scelti i colori più pesanti e i colori più leggeri
		// Viene anche scelto il colore più pesante
		int* nodes = new int[sol->n];
		int* nodeweight = new int[sol->n];
		for (i = 0; i < sol->tmax; i++) {
			cols[i] = i + 1;
			colweight[i] = 0;
		}
		for (i = 0; i < sol->n; i++) {
			nodes[i] = i;
			nodeweight[i] = 0;
		}
		penaltyByColor(sol, colweight, nodeweight);
		mergeSort(cols, colweight, sol->tmax);
		mergeSort(nodes, nodeweight, sol->tmax);

		for (i = 0; i < coso.second / 2; i++) // Colori più pesanti
			coso.first[i] = cols[sol->tmax - 1 - i];

		for (i = coso.second / 2; i < coso.second; i++) // Colori più leggeri
			coso.first[i] = cols[i - coso.second / 2];

		bool colore_nodo_pesante_in_lista = false;
		for (i = 0; i < coso.second && !colore_nodo_pesante_in_lista; i++) {
			if (coso.first[i] == sol->sol[nodes[0]]) {
				colore_nodo_pesante_in_lista = true;
			}
		}

		if (!colore_nodo_pesante_in_lista) {
			coso.first[coso.second - 1] = sol->sol[nodes[0]];
		}

		delete[] nodeweight;
		delete[] nodes;
		break;
	}

	delete[] colweight;
	delete[] cols;
	return coso;
}

void ColorShift::colorShiftRec(Solution* sol, std::pair<int*, int> cols, int* counters, int** exams_to_change, int node) {
	// Il colore di node = sol->sol[node];
	int i, j, k, colore = -1;
	bool nodo_da_vedere;

	// Aggiunta del nodo nella matrice exams_to_change
	for (j = 0; j < cols.second; j++) {
		if (cols.first[j] == sol->sol[node]) {
			colore = j;
			break;
		}
	}
	exams_to_change[colore][counters[colore]] = node;
	counters[colore]++;

	// Ricorsione sui nodi adiacenti
	for (i = 0; i < sol->n; i++) {
		if ( edge(node, i, *sol->g).second ) {
			for (j = 0; j < cols.second; j++) {
				if (cols.first[j] == sol->sol[i]) {
					nodo_da_vedere = true;

					// Controllo che i non sia già stato inserito in exams_to_change
					for (k = 0; k < counters[j] && nodo_da_vedere; k++) {
						if (exams_to_change[j][k] == i) {
							nodo_da_vedere = false;
						}
					}

					if (nodo_da_vedere) {
						colorShiftRec(sol, cols, counters, exams_to_change, i);
					}

					break;
				}
			}
		}
	}
}

void ColorShift::colorShift(Solution* sol, std::pair<int*, int> cols, int color_selection_mode) {
	srand(time(NULL) + clock());

	int initial_node = -1;
	bool newcolfirst;
	if (cols.first == NULL) {
		newcolfirst = true;
		cols = selectColors(sol, cols.second, color_selection_mode, &initial_node);
	}
	else {
		newcolfirst = false;
	}

	int* counters = new int[cols.second];
	int** exams_to_change = new int* [cols.second];
	int i, j, shift;

	for (i = 0; i < cols.second; i++) {
		counters[i] = 0;
		exams_to_change[i] = new int[sol->n];
	}

	if (initial_node == -1) {
		// Ricerca del nodo iniziale da cui far partire la ricorsione
		shift = rand() % sol->n;
		bool finito = false;
		for (i = 0; i < sol->n && !finito; i++) {
			for (j = 0; j < cols.second && !finito; j++) {
				if (cols.first[j] == sol->sol[(i + shift) % sol->n]) {
					// La ricorsione parte qui
					colorShiftRec(sol, cols, counters, exams_to_change, (i + shift) % sol->n);
					finito = true;
				}
			}
		}
	}
	else {
		colorShiftRec(sol, cols, counters, exams_to_change, initial_node);
	}

	// Cambiamento colori
	int newcol;
	shift = 1 + rand() % (cols.second - 1);
	for (i = 0; i < cols.second; i++) {
		newcol = cols.first[(i + shift) % cols.second];
		for (j = 0; j < counters[i]; j++) {
			sol->sol[exams_to_change[i][j]] = newcol;
		}
	}

	for (i = 0; i < cols.second; i++) {
		delete[] exams_to_change[i];
	}
	delete[] counters;
	delete[] exams_to_change;
	if (newcolfirst)
		delete[] cols.first;
}














