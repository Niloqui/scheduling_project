#ifndef ColorShift_hpp
#define ColorShift_hpp

#include "graphw.hpp"
#include "Solution.hpp"

class ColorShift {
private:
	static void penaltyByColor(Solution* sol, int* penalty);
	static void penaltyByColor(Solution* sol, int* penalty_by_color, int* penalty_by_exam);

	static std::pair<int*, int> selectColors(Solution* sol, int ncols, int selection_mode, int* initial_node);
	// Restituisce il vettore dei colori con la sua relativa lunghezza
	// La funzione non modifica sol
	// Se ncols == -1: Il numero di colori sarà scelto casualmente

	static void colorShiftRec(Solution *sol, std::pair<int*, int> cols, int* counters, int** exams_to_change, int node);

public:
	static void colorShift(Solution* sol, std::pair<int*, int> cols, int color_selection_mode);
	// Se cols.second == -1: Il numero di colori sarà scelto casualmente
	// color_selection_mode == 0: colori casuali
	// color_selection_mode == 1: colori più pesanti
	// color_selection_mode == 2: colori misti (metà pesanti e metà leggeri)
		// Viene messo forzatamente il nodo più pesante



};

#endif