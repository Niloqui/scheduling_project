#include "Solution.hpp"

#include <fstream>
#include "graphw.hpp"
#include "Utility.hpp"

using namespace std;

Solution::Solution(int n, int tmax) {
	this->n = n;
	this->tmax = tmax;
	this->sol = new int[n];
	optimum = false;
	indexexams = NULL;
	mask = NULL;

	for (int i = 0; i < n; i++)
		this->sol[i] = -1;
}

Solution::Solution(int n, int tmax, int* newsol) {
	this->n = n;
	this->tmax = tmax;
	this->sol = new int[n];
	optimum = false;
	indexexams = NULL;
	mask = NULL;

	setSolution(newsol);
}

void Solution::setSolution(Solution newsol, char car) {
	// car = ; 'm' -> mask; 'i' -> index
	switch (car){
	case 'a': // all
		for (int i = 0; i < this->n; i++)
			this->sol[i] = newsol.sol[i];
		break;

	case 'm': // mask
		for (int i = 0; i < this->n; i++)
			if (newsol.mask[i])
				this->sol[i] = newsol.sol[i];
		break;

	case 'i': // index
		for (int i = 0; i < newsol.n; i++)
			this->sol[newsol.indexexams[i]] = newsol.sol[i];
		break;
	}
}

void Solution::setSolution(int* newsol) {
	for (int i = 0; i < this->n; i++)
		this->sol[i] = newsol[i];
}

void Solution::setSolution(int* newsol, bool* mask) {
	for (int i = 0; i < this->n; i++)
		if(mask[i])
			this->sol[i] = newsol[i];
}

void Solution::setSolution(int* newsol, int* indexexams, int nsub) {
	for (int i = 0; i < nsub; i++)
		this->sol[indexexams[i]] = newsol[i];
}

int Solution::calculatePenalty(G::Graph g){
	int i, j, pen = 0, dist;
	pair<G::Edge, bool> e;

	for (i = 0; i < this->n && pen >= 0; i++) {
		if (this->sol[i] < 1 || this->sol[i] > tmax)
			pen = -1;

		for (j = i+1; j < this->n && pen >= 0; j++) {
			if (this->sol[j] < 1 || this->sol[j] > tmax)
				pen = -1;

			e = edge(i, j, g);
			if (e.second) {
				dist = distance(i, j);
				if (dist == 0)
					pen = -1;
				else if (dist <= 5)
					pen += integerPower(2, (5-dist)) * get(edge_weight_t(), g, e.first);
				// else if (dist > 5) {}
			}
		}
	}

	return pen;
}

int Solution::calculatePenalty(G::Graph g, int* indexvector, int nsub) {
	bool* mask = new bool[this->n];
	int pen;

	for (int i = 0; i < this->n; i++)
		mask[i] = false;

	for (int i = 0; i < nsub; i++)
		mask[indexvector[i]] = true;

	pen = calculatePenalty(g, mask);
	delete[] mask;
	return pen;
}

int Solution::calculatePenalty(G::Graph g, bool* mask) {
	int i, j, pen = 0, dist;
	pair<G::Edge, bool> e;

	for (i = 0; i < this->n && pen>=0; i++) {
		if (mask[i]) {
			if (this->sol[i] < 1 || this->sol[i] > tmax)
				pen = -1;

			for (j = i+1; j < this->n && pen >= 0; j++) {
				if (mask[j]){
					if (this->sol[j] < 1 || this->sol[j] > tmax)
						pen = -1;

					e = edge(i, j, g);
					if (e.second){
						dist = distance(i, j);
						if (dist == 0)
							pen = -1;
						else if (dist <= 5)
							pen += integerPower(2, dist) * get(edge_weight_t(), g, e.first);
						// else if (dist > 5) {}
					}
				}
			}
		}
	}

	return pen;
}

int Solution::distance(int i, int j) {
	int num1, num2;
	num1 = this->sol[i];
	num2 = this->sol[j];

	return num1 > num2 ? num1 - num2 : num2 - num1;
}

void Solution::printSolution(string filename) { // Esporta la soluzione su file
	string output = "";
	for (int i = 0; i < this->n; i++) {
		output += to_string(i + 1) + " " + to_string(sol[i]);
		if (i < (this->n - 1))
			output += "\n";
	}

	ofstream file(filename);
	file << output;
	file.close();
}


void Solution::printS() {
    for (int i = 0; i < this->n; i++) {
        std::cout << this->sol[i] << std::endl;
    }
}





















