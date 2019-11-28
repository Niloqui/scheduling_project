#include <iostream>
#include "rd.hpp"
#include "graphw.hpp"
#include "Solution.hpp"
#include <boost/graph/adjacency_list.hpp>

#include <ctime>

using namespace std;
using namespace boost;

int main(int argc, const char * argv[]) {
	clock_t start;
	double duration;
	if(argc!=2){
		std::cout <<"Wrong number of arguments passed"<< std::endl ;
		return -1;
	}

	start = clock();

	Reader r = Reader(argv[1]);
	G::Graph c = r.read(); //ritorna la matrice dei conlitti

	duration = (double)clock() - (double)start;
	cout << "Tempo impiegato per leggere il file: " + to_string( duration / CLOCKS_PER_SEC ) + " s\n";


    /*
    pair<G::Edge,bool> e = edge(3,13,c); //arco fra i vertici 3 e 13
     if(pair.second){ //vedere che il valore di second sia true, altrimenti non esiste l'arco!
        int w = get(edge_weight_t(),c,e.first); //come ricavare il peso del arco
        //peso = numero di studenti in comune
     }
     */

	/*
	// Stampa della matrice test
	pair<G::Edge, bool> e;
	int w;
	string str = "";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			e = edge(i, j, c);
			w = 0;
			if (e.second)
				w = get(edge_weight_t(), c, e.first);
			str += to_string(w);
			if ( j < 3 )
				str += ", ";
		}
		str += "\n";
	}
	cout << str;
	*/

	// Nome del file in uscita 
	string filename(argv[1]);
	filename += "_DMOgroup01.sol";

	
	if (strcmp(argv[1], "test") == 0) {
		Solution sol = Solution(r.getExamN(), r.getTmax());
		int* vect = new int[4];

		vect[0] = 0; vect[1] = 2; vect[2] = 5; vect[3] = 0;
		sol.setSolution(vect);
		cout << "penalita': " + to_string(sol.calculatePenalty(c)) + "\n";

		vect[0] = 1; vect[1] = 3; vect[2] = 6; vect[3] = 1;
		sol.setSolution(vect);
		cout << "penalita': " + to_string(sol.calculatePenalty(c)) + "\n";

		sol.printSolution(filename);
	}
	


	// TO-DO (forse): aggiungere deallocazione memoria


	cout << "Fine.";
	return 0;
}
