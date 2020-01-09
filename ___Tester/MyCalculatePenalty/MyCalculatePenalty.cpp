#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>

#include "graphw.hpp"
#include "rd.hpp"
#include "Solution.hpp"

using namespace std;

int main(int argc, const char* argv[]) {
	if (argc != 3) {
		// MyCalculatePenalty instancename instancesolution
		// In totale si riceveranno 2 parametri
		cout << "Wrong number of arguments passed.\n\t\tMyCalculatePenalty instancename instancesolution\n";
		return -1;
	}

	//// Lettura grafo
	Reader r = Reader(argv[1]);
	G::Graph g = r.read(); // Ritorna la matrice dei conlitti
	int studentNum = r.getStudents();
	int n = r.getExamN();
	int tmax = r.getTmax();

	//// Lettura soluzione
	Solution *sol = new Solution(n, tmax);
	ifstream file;
	string line(argv[2]);
	file.open(line + ".sol");
	pair<int, int> coppia;

	if (file.is_open()) {
		while (getline(file, line)) {
			if (line.length() > 1) { //dont count the last line, or empty ones
				std::istringstream sin(line);
				sin >> coppia.first >> coppia.second;

				sol->sol[coppia.first - 1] = coppia.second;
			}
		}
		file.close();
	}
	else {
		cout << "Unable to open initial solution. Error.\n";
		return -2;
	}
	
	sol->calculatePenalty(g);
	// cout << "Penalita' = " << sol->penalty / double(studentNum) << "\n";
	return sol->penalty;
}
