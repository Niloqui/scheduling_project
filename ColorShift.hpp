#ifndef ColorShift_hpp
#define ColorShift_hpp

#include "graphw.hpp"
#include "Solution.hpp"

class ColorShift {
private:
	static std::pair<int*, int> selectColors(Solution* sol, int ncols);
	// Restituisce il vettore dei colori con la sua relativa lunghezza
	// La funzione non modifica sol
	// Se ncols == -1: Il numero di colori sar� scelto casualmente

	static void colorShiftRec(Solution *sol, std::pair<int*, int> cols, int* counters, int** exams_to_change, int node);

public:
	static void colorShift(Solution *sol, std::pair<int*, int> cols);
	// Se cols.second == -1: Il numero di colori sar� scelto casualmente




};

#endif