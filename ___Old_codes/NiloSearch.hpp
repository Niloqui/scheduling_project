#ifndef NiloSearch_hpp
#define NiloSearch_hpp

#include "graphw.hpp"
#include "Solution.hpp"
#include <ctime>

class NiloSearch {
public:
	static void search(G::Graph* g, Solution* sol, clock_t tlim);
	// La funzione assume che sol sia feasible
	// tlim viene convertito da secondi a clock_per_sec all'interno di questa funzione

	static void LS(Solution* sol, Solution* temp, clock_t tlim, int attempts);
	// La funzione termina quando non c'è nessuna soluzione migliore nel vicinato;
	// La funzione termina prima se si raggiunge il tempo limite

	static bool isThereTime(clock_t tlim);




};


#endif