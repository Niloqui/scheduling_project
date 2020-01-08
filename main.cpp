#include <iostream>
#include "rd.hpp"
#include "graphw.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
#include "tabugroup/Tabu.hpp"
#include "InitialSolver.hpp"
#include "OptimalSolver.hpp"
#include "NiloSearch.hpp"
#include <thread>
#include <boost/graph/connected_components.hpp>
#include <ctime>

//Kempe search
#include "kempegroup/Kempe.hpp"

//Tabu search
#include "tabugroup/Tabu.hpp"

//Local search
#include "LocalSearchGroup/LocalSearch.hpp"

using namespace std;
using namespace boost;

int getSubProblem(G::Graph *g, Solution **subsol, int n, int tmax);
// Restituisce il numero di sottoproblemi
// Viene allocato il vettore sub
void solver(G::Graph *g, Solution* sol, int tlim);

clock_t start;

int main(int argc, const char * argv[]) {
    clock_t begin;
    double duration;
	int tmax, n , tlim, studentNum;
    
    begin = clock();

	if(argc!=4){
		// In totale si riceveranno 3 parametri
		// ETPsolver_DMOgroup01.exe instancename -t tlim
		cout <<"Wrong number of arguments passed.\nPlease call the program with the format:\tETPsolver DMOgroupXX.exe instancename -t tlim" << endl;
	return -1;
	}
	// Tempo limite
	// argv[2] è semplicemente -t
	tlim = atoi(argv[3]);

	//// Nome del file in uscita
	string filename(argv[1]);
	filename += "_DMOgroup01.sol";

	//// Lettura file
	Reader r = Reader(argv[1]);
	G::Graph c = r.read(); // Ritorna la matrice dei conlitti
	studentNum = r.getStudents();
	n = r.getExamN();
	tmax = r.getTmax();

	/*
	int n = num_vertices(c);
	pair<G::Edge, bool> e;
	for (int i = 0; i < n; i++){
		e = edge(0, i, c);
		cout << "Arco (0, " + to_string(i) + ") = " + (e.second? to_string(get(edge_weight_t(), c, e.first)):"False") << endl;
	}
	*/

	/*
	pair<G::Edge,bool> e = edge(3,13,c); //arco fra i vertici 3 e 13
	if(pair.second){ //vedere che il valore di second sia true, altrimenti non esiste l'arco!
		int w = get(edge_weight_t(),c,e.first); //come ricavare il peso del arco
		//peso = numero di studenti in comune
	}
	*/

	//// Inizio soluzione
	//Solution mothersolution(n, tmax);
    // Soluzione dell'intera istanza
	// srand(time(NULL) + (unsigned)&c + tlim);
	// Se serve generare dei valori casuali, è utile usare srand solo una volta e all'inizio
	// mothersolution.indexexams = new int[n];
	// for (i = 0; i < n; i++)
		// mothersolution.indexexams[i] = i;

	/*
	int esame = 0;
	G::AdjacencyIterator ai, a_end;
	tie(ai, a_end) = adjacent_vertices(esame, c); // Vertici adiacenti a esame
	for (; ai != a_end; ai++) {
		std::cout << *ai << "\t";
	}
	*/


	vector<int> component(num_vertices(c));
	cout << "Numero di vertici: " + to_string(num_vertices(c)) + "\n\n";

	//////////////////// Inizio soluzione
	n = r.getExamN();
	tmax = r.getTmax();
	Solution master(n, tmax); // Soluzione dell'intera istanza

	//// Creazione sottoproblemi
    /*
	Solution* subsol; // Vettore dei sottoproblemi
	int num_components = getSubProblem(&c, &subsol, n, tmax);
	thread **tred = new thread*[num_components];
	for (i = 0; i < num_components; i++)
		tred[i] = new thread(solver, &c, &subsol[i], tlim); // implementare tlim
	
<<<<<<< HEAD
	//// Attesa della chiusura dei thread
	for (i = 0; i < num_components; i++)
		tred[i]->join();

	//// Esport della soluzione
	for (int i = 0; i < num_components; i++)
		mothersolution.setSolution(subsol[i], true);
	mothersolution.printSolution(filename);*/


	
	start = clock();

	Solution sol(n, tmax);
	int* res = new int[n];
	int* indexvector = new int[n];
	int* pesi = new int[n];
	for (int i = 0; i < n; i++) {
		indexvector[i] = i;
		pesi[i] = 0;

		for (int j = 0; j < n; j++)
			if (edge(i, j, c).second)
				pesi[i]++;
	}
	mergeSort(indexvector, pesi, n);
	reverseVector(indexvector, n);

	pair<int, bool> coso = InitialSolver::squeakyWheel(c, res, indexvector, n, tmax);
    
    
	sol.setSolution(res, indexvector, n);
	cout << "Penalita': " << to_string(sol.calculatePenaltyFull(c,studentNum)) << "\n\n";

	string output = "Numero di time slot utilizzati = " + to_string(coso.first) + "\n";
	output += "tmax = " + to_string(r.getTmax()) + "\t\tn = " + to_string(r.getExamN()) + "\n";
	cout << output;

	duration = (double)clock() - (double)start;
	cout << "\n\nTempo impiegato per leggere lo squeakyWheel: " + to_string(duration / CLOCKS_PER_SEC) + " s\n\n";
    duration = duration/CLOCKS_PER_SEC;
     
    /*----------Soluzione tabu-------------**/
        
    Tabu tab(10,c,sol,studentNum);
    int tollerance = 5;
    tab.tabuIteratedLocalSearch(c, sol, tollerance, begin, tlim,duration*2,filename);
    sol.printSolution(filename);


    double penalita = sol.calculatePenaltyFull(c,studentNum);
    cout << "\n\nPenalita': " << to_string(penalita) << "\n\n";
    
    duration = (double)(clock()) / CLOCKS_PER_SEC;
    output = "\nTempo impiegato per risolvere il problema: " + to_string(duration) + "\n";
	cout << output << "tlim = " << tlim << endl;

	return 0;
}

/*
void solver(G::Graph *g, Solution *sol, int tlim) {
	//string output;

	if (OptimalSolver::problemIsReallySmall(sol)) {
		OptimalSolver::solveReallySmallProblem(sol);
		//output = "Molto piccolo " + to_string(sol->n) + "\n";
		//cout << output;
	}
	else if (OptimalSolver::problemIsSmall(sol, MAX_O)) {
		//clock_t begin = clock();
		OptimalSolver::solveSmallProblem(g, sol);
		//double duration = (double)(clock() - begin) / CLOCKS_PER_SEC;
		//output = "Piccolo " + to_string(sol->n) + "\n\tTempo impiegato per il risolutore ottimo: " + to_string(duration) + "\n";
		//cout << output;
	}
	else { // Il problema non è piccolo
		InitialSolver::squeakyWheel(*g, sol); // Soluzione iniziale

		NiloSearch::search(g, sol, tlim);

		///////////////
		//////////////////////////////////////// Euristiche
		///////////////

		//output = "Non piccolo " + to_string(sol->n) + "\n";
		//cout << output;
	}
}


int getSubProblem(G::Graph *g, Solution** subsol, int n, int tmax) {
	vector<int> component(n);
	int num_components = connected_components(*g, &component[0]);

	*subsol = new Solution[num_components];
	Solution *sol;
	int** indexvectors = new int* [num_components];
	int* counters = new int[num_components];
	int i, j;

	for (i = 0; i < num_components; i++) {
		counters[i] = 0;
		indexvectors[i] = new int[n];
	}

	for (j = 0; j < n; j++) {
		i = component[j];
		indexvectors[i][counters[i]] = j;
		counters[i]++;
	}

	for (i = 0; i < num_components; i++) {
		sol = &(*subsol)[i];
		sol->n = counters[i];
		sol->tmax = tmax;
		sol->indexexams = new int[counters[i]];
		sol->sol = new int[counters[i]];

		for (j = 0; j < counters[i]; j++) {
			sol->sol[j] = -1;
			sol->indexexams[j] = indexvectors[i][j];
		}
	}

	return num_components;
}
*/


