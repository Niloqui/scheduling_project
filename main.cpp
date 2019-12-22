#include <iostream>
#include "rd.hpp"
#include "graphw.hpp"
#include "Solution.hpp"
#include "Utility.hpp"
#include "Tabu.hpp"
#include "InitialSolver.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

//Kempe search
#include "Kempe.hpp"

//Tabu search
#include "tabugroup/Tabu.hpp"

//Local search
#include "LocalSearch.hpp"
#include <ctime>


using namespace std;
using namespace boost;


clock_t start;

int main(int argc, const char * argv[]) {
    clock_t begin;
	double duration;
	int tmax, n,studentNum,tlim;
    begin = clock();

	if(argc!=4){
		// TO-DO: In totale si riceveranno 3 parametri, non 1 solo
		// ETPsolver_DMOgroup01.exe instancename -t tlim
		cout <<"Wrong number of arguments passed"<< endl ;
        cout <<"Please call the program with the format:"<< endl;
        cout <<"ETPsolver DMOgroupXX.exe instancename -t tlim"<< endl;
		return -1;
	}
    //tempo limite
    //argv[2] è semplcemente -t
    tlim = atoi(argv[3]);
    
	// Nome del file in uscita 
	string filename(argv[1]);
	filename += "_DMOgroup01.sol";

	//////////////////// Lettura file
	start = clock();

	Reader r = Reader(argv[1]);
	G::Graph c = r.read(); //ritorna la matrice dei conlitti
    studentNum = r.getStudents();


	//////////////////// Componenti connesse
	start = clock();

	vector<int> component(num_vertices(c));
	cout << "Numero di vertici: " + to_string(num_vertices(c)) + "\n\n";

	//////////////////// Inizio soluzione
	n = r.getExamN();
	tmax = r.getTmax();
	Solution master(n, tmax); // Soluzione dell'intera istanza

	
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
	cout << "Penalita': " << to_string(sol.calculatePenaltyFull(c,studentNum)) << "\n\n";

	string output = "Numero di time slot utilizzati = " + to_string(coso.first) + "\n";
	output += "tmax = " + to_string(r.getTmax()) + "\t\tn = " + to_string(r.getExamN()) + "\n";
	cout << output;

	duration = (double)clock() - (double)start;
	cout << "\n\nTempo impiegato per leggere lo squeakyWheel: " + to_string(duration / CLOCKS_PER_SEC) + " s\n\n";
    
    /*--------------------------------------------**/
    /*----------Soluzione tabu-------------**/
    
    
    colorGraph(c, sol);
    Tabu tab(10,c,sol);
    int tollerance = 5;
    tab.tabuIteratedLocalSearch(c, sol, tollerance, begin, tlim);
    
     
    /*Soluzione iterated local search*/
    //cout << "Tempo all'iterated: " << to_string(clock()/CLOCKS_PER_SEC) << endl;
    //iteratedLocalSearch(c, sol, 1, begin, tlim);
    
    sol.printSolution(filename);

    
    double penalita = sol.calculatePenaltyFull(c,studentNum);
    cout << "\n\nPenalita': " <<
    to_string(penalita) << "\n\n";



	return 0;
}






