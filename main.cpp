#include <iostream>
#include "rd.hpp"
#include "graphw.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
// #include "tabugroup/Tabu.hpp"
#include "InitialSolver.hpp"
// #include "OptimalSolver.hpp"
// #include "NiloSearch.hpp"
#include <thread>
// #include <boost/graph/adjacency_list.hpp>
// #include <boost/graph/connected_components.hpp>
#include <ctime>

#include "ColorShift.hpp"

//Kempe search
// #include "kempegroup/Kempe.hpp"

//Local search
// #include "LocalSearchGroup/LocalSearch.hpp"

// #define NUM_CORES (std::thread::hardware_concurrency())
#define NUM_CORES 8
#define NUM_INITIAL_PERTURBATION 10

using namespace std;
using namespace boost;

void solver(G::Graph* g, Solution* sol, int tlim, Solution* mothersolution);

int main(int argc, const char * argv[]) {
	int tmax, n, i, j, tlim, studentNum;

	if (argc!=4) {
		// In totale si riceveranno 3 parametri
		// ETPsolver_DMOgroup01.exe instancename -t tlim
		cout <<"Wrong number of arguments passed.\nPlease call the program with the format:\tETPsolver DMOgroupXX.exe instancename -t tlim" << endl;
		return -1;
	}

	//// Tempo limite
	// argv[2] è semplicemente -t
	tlim = atoi(argv[3]);

	//// Nome del file in uscita
	string filename(argv[1]);
	filename += "_DMOgroup01.sol";

	//// Random seed. Valido solo per il main
	srand(time(NULL));

	//// Lettura file
	Reader r = Reader(argv[1]);
	G::Graph c = r.read(); // Ritorna la matrice dei conlitti
	studentNum = r.getStudents();
	n = r.getExamN();
	tmax = r.getTmax();

	//// Inizio soluzione
	Solution* mothersolution = new Solution(&c, n, tmax); // Soluzione dell'intera istanza
	mothersolution->filename = filename;
	Solution* temp = new Solution(&c, n, tmax);
	InitialSolver::squeakyWheel(temp);

	//// Creazione thread
	Solution** subsol = new Solution * [NUM_CORES];
	thread** treds = new thread * [NUM_CORES];

	for (i = 0; i < NUM_CORES; i++) {
		if (i != 0) {
			// Perturbazione temp
			std::pair<int*, int> cols;
			cols.first = NULL;
			for (j = 0; j < NUM_INITIAL_PERTURBATION; j++) {
				cols.second = 2 + rand() % 4; // minimo 2, massimo 5
				ColorShift::colorShift(temp, cols);
			}
		}
		
		subsol[i] = new Solution(temp);
		// subsol[i]->g = copia del grafo originale;
		treds[i] = new thread(solver, &c, subsol[i], tlim, mothersolution);
	}

	//// Attesa della chiusura dei thread
	for (i = 0; i < NUM_CORES; i++) {
		treds[i]->join();
		mothersolution->setSolutionAndPrint(subsol[i]);
	}


	//// Deallocazione memoria
	// TO-DO (forse): aggiungere deallocazione memoria



	mothersolution->calculatePenalty();

	double duration = (double)(clock()) / CLOCKS_PER_SEC;
	string output(argv[1]);
	output = "\n" + output + "\nTempo impiegato per risolvere il problema: " + to_string(duration) + "\n";
	cout << output << "tlim = " << tlim << "\npenalita' = " << (double)(mothersolution->penalty) / r.getStudents();

	return mothersolution->penalty;
}

void solver(G::Graph* g, Solution* sol, int tlim, Solution* mothersolution) {
	mothersolution->setSolutionAndPrint(sol);

	/////////////////////////////// Euristica

}






