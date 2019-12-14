#include <iostream>
#include "rd.hpp"
#include "graphw.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
#include "InitialSolver.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

//Kempe search
#include "kempegroup/Kempe.hpp"

#include <ctime>

using namespace std;
using namespace boost;

clock_t start;

int main(int argc, const char * argv[]) {
	double duration;
	int tmax, n;

	if(argc!=2){
		// TO-DO: In totale si riceveranno 3 parametri, non 1 solo
		// ETPsolver_DMOgroup01.exe instancename -t tlim
		cout <<"Wrong number of arguments passed"<< endl ;
		return -1;
	}

	// Nome del file in uscita 
	string filename(argv[1]);
	filename += "_DMOgroup01.sol";

	//////////////////// Lettura file
	start = clock();

	Reader r = Reader(argv[1]);
	G::Graph c = r.read(); //ritorna la matrice dei conlitti

	duration = (double)clock() - (double)start;
	cout << "Tempo impiegato per leggere il file: " + to_string( duration / CLOCKS_PER_SEC ) + " s\n\n";

	//////////////////// Componenti connesse
	start = clock();

	vector<int> component(num_vertices(c));
	int num_components = connected_components(c, &component[0]);
	cout << "Numero di vertici: " + to_string(num_vertices(c)) + "\n\n";

	duration = (double)clock() - (double)start;
	cout << "Tempo impiegato per ottenere le componenti connesse: " + to_string(duration / CLOCKS_PER_SEC) + " s\n\n";



	//////////////////// Inizio soluzione
	n = r.getExamN();
	tmax = r.getTmax();
	Solution master(n, tmax); // Soluzione dell'intera istanza
	Solution* subsol;

	//////////////////// Sottosoluzioni delle componenti connesse
	if (num_components > 1) {
		subsol = new Solution[num_components];
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
			subsol[i].n = counters[i];
			subsol[i].tmax = tmax;
			subsol[i].indexexams = new int[counters[i]];
			subsol[i].sol = new int[counters[i]];

			for (j = 0; j < counters[i]; j++) {
				subsol[i].sol[j] = -1;
				subsol[i].indexexams[j] = indexvectors[i][j];
			}
		}
	}
	else { // num_components == 1
		subsol = &master;
	}
	
	//////////////////////////////////
	//////////////////////////////////
	//////////////////////////////////
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
	cout << "\n\n" << sol.printSolution(filename) << "\n\nPenalita': " << to_string(sol.calculatePenalty(c)) << "\n\n";

	string output = "Numero di time slot utilizzati = " + to_string(coso.first) + "\n";
	output += "tmax = " + to_string(r.getTmax()) + "\t\tn = " + to_string(r.getExamN()) + "\n";
	cout << output;

	duration = (double)clock() - (double)start;
	cout << "\n\nTempo impiegato per leggere lo squeakyWheel: " + to_string(duration / CLOCKS_PER_SEC) + " s\n\n";


    
    /*--------------------------------------------**/
    /*----------Soluzione tabu search-------------**/
    colorGraph(c,sol);
    G::Vertex v = vertex(2, c);
    cout << "Penalità mossa: " << kempeMovePenaltyWrapper(c, v, 12) << endl;
    
    simpleKempe(c,v,12);
    setSolution(c, sol);
    
    cout << "\n\nPenalita': " <<
    to_string(sol.calculatePenalty(c)) << "\n\n";



	return 0;
}







