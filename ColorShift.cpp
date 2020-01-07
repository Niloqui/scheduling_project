#include "ColorShift.hpp"

#include <ctime>
#include <iostream>
#include <cstdlib>
#include "Utility.hpp"

std::pair<int*, int> ColorShift::selectColors(Solution* sol, int ncols) {
	std::pair<int*, int> coso;
	if (ncols == -1)
		// coso.second = (int)( ((float)rand() / RAND_MAX) * ((sol->tmax * 5.0) / 7.0) ) + 2;
		coso.second = (int)(((float)rand() / RAND_MAX) * (sol->tmax * 0.4)) + 2;
	else
		coso.second = ncols;
	
	coso.first = new int[coso.second];
	int* cols = new int[(int64_t)sol->tmax + 1];
	int* colweight = new int[sol->tmax];
	int i;

	// I colori sono pesati a caso
	// forse TO-DO: mettere i pesi in base alla penalità
	for (i = 0; i < sol->tmax; i++) {
		cols[i] = i + 1;
		colweight[i] = rand();
	}
	mergeSort(cols, colweight, sol->tmax);

	// Per ora i colori vengono scelti casualmente
	for (int i = 0; i < coso.second; i++)
		coso.first[i] = cols[i];

	delete[] cols;
	delete[] colweight;
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

void ColorShift::colorShift(Solution* sol, std::pair<int*, int> cols) {
	srand(time(NULL) + clock());

	bool newcolfirst;
	if (cols.first == NULL) {
		newcolfirst = true;
		cols = selectColors(sol, cols.second);
	}
	else {
		newcolfirst = false;
	}

	int* counters = new int[cols.second];
	int** exams_to_change = new int*[cols.second];
	int i, j, shift;

	for (i = 0; i < cols.second; i++) {
		counters[i] = 0;
		exams_to_change[i] = new int[sol->n];
	}

	// Ricerca del nodo iniziale da cui far partire la ricorsione
	shift = rand() % sol->n;
	bool finito = false;
	for (i = 0; i < sol->n && !finito; i++) {
		for (j = 0; j < cols.second && !finito; j++) {
			if ( cols.first[j] == sol->sol[(i + shift) % sol->n] ) {
				// La ricorsione parte qui
				colorShiftRec(sol, cols, counters, exams_to_change, (i + shift) % sol->n);
				finito = true;
			}
		}
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
	if(newcolfirst)
		delete[] cols.first;
}














