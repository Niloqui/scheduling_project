#ifndef ColorShift_hpp
#define ColorShift_hpp

#include "graphw.hpp"
#include "Solution.hpp"

class ColorShift {
private:
	static void penaltyByColor(Solution* sol, int *penalty);
	static std::pair<int*, int> selectColors(Solution* sol, int ncols, bool random_colors);
	// Restituisce il vettore dei colori con la sua relativa lunghezza
	// La funzione non modifica sol
	// Se ncols == -1: Il numero di colori sarà scelto casualmente

	static void colorShiftRec(Solution *sol, std::pair<int*, int> cols, int* counters, int** exams_to_change, int node);

public:
	static void colorShift(Solution *sol, std::pair<int*, int> cols, bool random_colors);
	// Se cols.second == -1: Il numero di colori sarà scelto casualmente




};

#endif