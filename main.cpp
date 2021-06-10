#include <iostream>
#include <thread>

#include "rd.hpp"
#include "ColorShift.hpp"
#include "InitialSolver.hpp"
#include "tabugroup/Tabu.hpp" // Tabu Search

// #define NUM_CORES 8
#define NUM_CORES (std::thread::hardware_concurrency())
#define NUM_INITIAL_PERTURBATION 10

using namespace std;

void solver(G::Graph* g, Solution* sol, TimeController* tlim, Solution* mothersolution, int studentNum, int tollerance);

clock_t start;

int main(int argc, const char * argv[]) {
	int tmax, n, i, j, studentNum, num_cores = NUM_CORES, tollerance = 5;

	if (argc != 4) {
		// In totale si riceveranno 3 parametri
		// ETPsolver_DMOgroup01.exe instancename -t tlim
		cout << "Wrong number of arguments passed.\nPlease call the program with the following format:\n";
		cout << "\tETPsolver DMOgroupXX.exe instancename -t tlim\n";
		return -1;
	}

	//// Tempo limite
	// argv[2] è semplicemente -t
	// tlim = atoi(argv[3]);
	TimeController tlim(atoi(argv[3]));

	//// Nome del file in uscita
	string filename(argv[1]);
	filename += "_DMOgroup01.sol";

	//// Random seed. Valido solo per il main
	srand((unsigned int)time(NULL));

	//// Lettura file
	Reader r = Reader(argv[1]);
	G::Graph c = r.read(); // Ritorna la matrice dei conlitti
	studentNum = r.getStudents();
	n = r.getExamN();
	tmax = r.getTmax();

	//// Inizio soluzione
	Solution* mothersolution = new Solution(n, tmax); // Soluzione dell'intera istanza
	mothersolution->filename = filename;
	Solution* temp = new Solution(n, tmax);
	InitialSolver::squeakyWheel(&c, temp);
	
	//// Creazione thread
	Solution** subsol = new Solution * [num_cores];
	G::Graph **graphs = new G::Graph*[num_cores];
	thread** treds = new thread * [num_cores];

	for (i = 0; i < num_cores; i++) {
		if (i != 0) { // Perturbazione temp
			std::pair<int*, int> cols;
			cols.first = NULL;
			for (j = 0; j < NUM_INITIAL_PERTURBATION; j++) {
				cols.second = 3 + rand() % 4; // minimo 3, massimo 6
				ColorShift::colorShift(&c, temp, cols, j % 3);
			}

			ColorShift::totalColorShift(temp);
		}
		subsol[i] = new Solution(temp);

		graphs[i] = new G::Graph;
		copy_graph(c, *graphs[i]);

		// void solver(G::Graph* g, Solution* sol, TimeController* tlim, Solution* mothersolution, int studentNum, int tollerance)
		treds[i] = new thread(solver, graphs[i], subsol[i], &tlim, mothersolution, studentNum, tollerance);
	}

	//// Attesa della chiusura dei thread
	for (i = 0; i < num_cores; i++) {
		treds[i]->join();
	}

	//// Deallocazione memoria
	for (i = 0; i < num_cores; i++){
		delete[] subsol[i]->sol;
		graphs[i]->clear();
	}
	delete[] treds;
	delete[] graphs;
	delete[] subsol;

	i = mothersolution->calculatePenalty(c);
	c.clear();
	delete[] temp->sol;
	delete[] mothersolution->sol;
	delete temp;
	delete mothersolution;

	return i; // La penalità non viene divisa dal numero di studenti
}

void solver(G::Graph* g, Solution* sol, TimeController* tlim, Solution* mothersolution, int studentNum, int tollerance) {
	sol->calculatePenalty(*g);
	string pippo = mothersolution->checkSetPrintSolution(g, sol);


	int tabu_length = 10;
	Tabu tab(tabu_length, *g, *sol, studentNum);

	tab.tabuIteratedLocalSearch(*g, *sol, tollerance, *tlim, *mothersolution);
}
