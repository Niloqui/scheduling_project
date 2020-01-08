#ifndef ColorShift_hpp
#define ColorShift_hpp

#include "graphw.hpp"
#include "Solution.hpp"

class ColorShift {
private:
	static void penaltyByColor(G::Graph* g, Solution* sol, int* penalty);
	static void penaltyByColor(G::Graph* g, Solution* sol, int* penalty_by_color, int* penalty_by_exam);

	static std::pair<int*, int> selectColors(G::Graph* g, Solution* sol, int ncols, int selection_mode, int* initial_node);
	// Restituisce il vettore dei colori con la sua relativa lunghezza
	// La funzione non modifica sol
	// Se ncols == -1: Il numero di colori sar� scelto casualmente

	static void colorShiftRec(G::Graph* g, Solution *sol, std::pair<int*, int> cols, int* counters, int** exams_to_change, int node);

public:
	static void colorShift(G::Graph* g, Solution* sol, std::pair<int*, int> cols, int color_selection_mode);
	// Se cols.second == -1: Il numero di colori sar� scelto casualmente
	// color_selection_mode == 0: colori casuali
	// color_selection_mode == 1: colori pi� pesanti
	// color_selection_mode == 2: colori misti (met� pesanti e met� leggeri)
		// Viene messo forzatamente il nodo pi� pesante



};

#endif