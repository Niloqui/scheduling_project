#include <iostream>
#include "rd.hpp"
#include "graphw.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
#include "InitialSolver.hpp"
#include "OptimalSolver.hpp"
#include <thread>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#include <ctime>

#define MAX_O 1000000000
// MAX_O serve come massimo numero di operazioni da eseguire in caso di risolutore ottimale

using namespace std;
using namespace boost;

int getSubProblem(G::Graph g, Solution **subsol, int n, int tmax);
// Restituisce il numero di sottoproblemi
// Viene allocato il vettore sub
void solver(G::Graph g, Solution* sol);

const clock_t start = clock();

int main(int argc, const char * argv[]) {
	int tmax, n, i;

	if (argc != 2) {
		// TO-DO: In totale si riceveranno 3 parametri, non 1 solo
		// ETPsolver_DMOgroup01.exe instancename -t tlim
		cout << "Wrong number of arguments passed\n";
		return -1;
	}

	//// Nome del file in uscita 
	string filename(argv[1]);
	filename += "_DMOgroup01.sol";

	//// Lettura file
	Reader r = Reader(argv[1]);
	G::Graph c = r.read(); // Ritorna la matrice dei conlitti

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
	n = r.getExamN();
	tmax = r.getTmax();
	Solution master(n, tmax); // Soluzione dell'intera istanza

	//// Creazione sottoproblemi
	Solution* subsol; // Vettore dei sottoproblemi
	int num_components = getSubProblem(c, &subsol, n, tmax);
	thread* tred = new thread[num_components]();
	for (i = 0; i < num_components; i++)
		tred[i] = thread(solver, c, &subsol[i]);
	
	//// Attesa della chiusura dei thread
	for (i = 0; i < num_components; i++)
		tred[i].join();

	//// Esport della soluzione
	for (int i = 0; i < num_components; i++)
		master.setSolution(subsol[i], true);
	master.printSolution(filename);


	//// Deallocazione memoria
	// TO-DO (forse): aggiungere deallocazione memoria


	return 0;
}

void solver(G::Graph g, Solution *sol) {
	string output;

	if (OptimalSolver::problemIsReallySmall(sol)) {
		OptimalSolver::solveReallySmallProblem(g, sol);
		output = "Molto piccolo " + to_string(sol->n) + "\n";
		cout << output;
	}
	else if (OptimalSolver::problemIsSmall(sol, MAX_O)) {
		OptimalSolver::solveSmallProblem(g, sol);
		output = "Piccolo " + to_string(sol->n) + "\n";
		cout << output;
	}
	else { // Il problema non � piccolo
		InitialSolver::squeakyWheel(g, sol); // Soluzione iniziale

		///////////////
		//////////////////////////////////////// Euristiche
		///////////////

		output = "Non piccolo " + to_string(sol->n) + "\n";
		cout << output;
	}
}


int getSubProblem(G::Graph g, Solution** subsol, int n, int tmax) {
	vector<int> component(n);
	int num_components = connected_components(g, &component[0]);

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




