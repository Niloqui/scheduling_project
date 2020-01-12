#include "Solution.hpp"

#include <fstream>
#include <mutex>

using namespace std;
mutex mtx;

Solution::Solution(int n, int tmax) {
	this->n = n;
	this->tmax = tmax;
	sol = new int[n];

	for (int i = 0; i < n; i++)
		sol[i] = -1;
}

Solution::Solution(Solution* sol) {
	this->n = sol->n;
	this->tmax = sol->tmax;
	this->penalty = sol->penalty;

	this->sol = new int[sol->n];
	for (int i = 0; i < this->n; i++)
		this->sol[i] = sol->sol[i];
}

void Solution::setSolution(Solution* newsol) {
	for (int i = 0; i < this->n; i++)
		this->sol[i] = newsol->sol[i];
}

int Solution::calculatePenalty(G::Graph& g) { // Usare adjacency_list per trovare i nodi adiacenti
	int i, j, dist;
	pair<G::Edge, bool> e;
	
	penalty = 0;
	for (i = 0; i < this->n && penalty >= 0; i++) {
		if (this->sol[i] < 1 || this->sol[i] > tmax)
			penalty = -1;

		for (j = i+1; j < this->n && penalty >= 0; j++) {
			if (this->sol[j] < 1 || this->sol[j] > tmax)
				penalty = -1;

			e = edge(i, j, g);
			if (e.second && penalty >= 0) {
				dist = distance(i, j);
				if (dist == 0)
					penalty = -1;
				else if (dist <= 5)
					penalty += (1 << (5 - dist)) * get(edge_weight_t(), g, e.first);
				// else if (dist > 5) {}
			}
		}
	}

	return penalty;
}

int Solution::distance(int i, int j) {
	int num1, num2;
	num1 = this->sol[i];
	num2 = this->sol[j];

	return num1 > num2 ? num1 - num2 : num2 - num1;
}

string Solution::checkSetPrintSolution(G::Graph* g, Solution* sol) {
	string output = "";

	mtx.lock();
	if (this->penalty < 0 || sol->penalty < this->penalty) {
		this->setSolution(sol);
		this->calculatePenalty(*g);
		
		for (int i = 0; i < this->n; i++)
			output += to_string(i + 1) + " " + to_string(this->sol[i]) + "\n";

		ofstream file(this->filename);
		file << output;
		file.close();
	}
	mtx.unlock();

	return output;
}
