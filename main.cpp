#include <iostream>
#include "rd.hpp"
#include "graphw.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
#include "InitialSolver.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#include <ctime>

using namespace std;
using namespace boost;

clock_t start;

int main(int argc, const char * argv[]) {
	double duration;

	if(argc!=2){
		// TO-DO: In totale si riceveranno 3 parametri, non 1 solo
		// ETPsolver_DMOgroup01.exe instancename -t tlim
		std::cout <<"Wrong number of arguments passed"<< std::endl ;
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
	size_t num_components = connected_components(c, &component[0]);
	cout << "Numero di vertici: " + to_string(num_vertices(c)) + "\n\n";

	duration = (double)clock() - (double)start;
	cout << "Tempo impiegato per ottenere le componenti connesse: " + to_string(duration / CLOCKS_PER_SEC) + " s\n\n";

	for (int i = 0; i < num_components; i++){
		cout << "Vertici nella componente #" + to_string(i) + ": " << endl;
		for (int j = 0; j < num_vertices(c); j++) {
			if (component[j] == i)
				std::cout << j+1 << " ";
		}

		cout << "\n\n";
	}


	//////////////////// firstPossiblePosition
	start = clock();

	int n = r.getExamN(), tmax = r.getTmax();
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

	pair<int, bool> coso = InitialSolver::firstPossiblePosition(c, res, indexvector, n, tmax);
	sol.setSolution(res, indexvector, n);
	cout << "\n\n" << sol.printSolution(filename) << "\n\nPenalita': " << to_string(sol.calculatePenalty(c) << "\n\n";

	string output = "Numero di time slot utilizzati = " + to_string(coso.first) + "\n";
	output += "tmax = " + to_string(r.getTmax()) + "\t\tn = " + to_string(r.getExamN()) + "\n";
	cout << output;

	string str(argv[1]);
	output = str + "\n\n" + output + "\n\n\n";
	str += "_info.txt";
	ofstream file(str);
	file << output;
	file.close();

	duration = (double)clock() - (double)start;
	cout << "\n\nTempo impiegato per leggere il firstPossiblePosition: " + to_string(duration / CLOCKS_PER_SEC) + " s\n\n";
	



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




	// TO-DO (forse): aggiungere deallocazione memoria


	cout << "Fine.";
	return 0;
}





