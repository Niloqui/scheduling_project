#include "Solution.hpp"

#include <fstream>
#include "graphw.hpp"
#include "Utility.hpp"
#include <iostream>
#include <mutex>          // std::mutex, std::unique_lock

std::mutex mtx;           // mutex for critical section

using namespace std;

Solution::Solution() {}

Solution::Solution(int n, int tmax) {
	this->n = n;
	this->tmax = tmax;
	sol = new int[n];

	for (int i = 0; i < n; i++)
		sol[i] = -1;
}

Solution::Solution(int n, int tmax, int* newsol) {
	this->n = n;
	this->tmax = tmax;
	sol = new int[n];

	setSolution(newsol);
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

void Solution::setSolution(int* newsol) {
	for (int i = 0; i < this->n; i++)
		this->sol[i] = newsol[i];
}

void Solution::setSolution(int* newsol, int* indexexams, int nsub) {
	for (int i = 0; i < nsub; i++)
		this->sol[indexexams[i]] = newsol[i];
}

int Solution::calculatePenalty(G::Graph g) {
	int i, j, dist;
	pair<G::Edge, bool> e;

	penalty = 0;
	for (i = 0; i < this->n && penalty >= 0; i++) {
		if (this->sol[i] < 1 || this->sol[i] > tmax)
			penalty = -1;

		for (j = i + 1; j < this->n && penalty >= 0; j++) {
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

int Solution::calculatePenalty(G::Graph* g) { // Usare adjacency_list per trovare i nodi adiacenti
	int i, j, dist;
	pair<G::Edge, bool> e;
	
	penalty = 0;
	for (i = 0; i < this->n && penalty >= 0; i++) {
		if (this->sol[i] < 1 || this->sol[i] > tmax)
			penalty = -1;

		for (j = i+1; j < this->n && penalty >= 0; j++) {
			if (this->sol[j] < 1 || this->sol[j] > tmax)
				penalty = -1;

			e = edge(i, j, *g);
			if (e.second && penalty >= 0) {
				dist = distance(i, j);
				if (dist == 0)
					penalty = -1;
				else if (dist <= 5)
					penalty += (1 << (5 - dist)) * get(edge_weight_t(), *g, e.first);
				// else if (dist > 5) {}
			}
		}
	}

	return penalty;
}

int Solution::calculatePenalty(G::Graph* g, bool* mask) {
	int i, j, dist;
	pair<G::Edge, bool> e;

	penalty = 0;
	for (i = 0; i < this->n && penalty >=0; i++) {
		if (mask[i]) {
			if (this->sol[i] < 1 || this->sol[i] > tmax)
				penalty = -1;

			for (j = i+1; j < this->n && penalty >= 0; j++) {
				if (mask[j]){
					if (this->sol[j] < 1 || this->sol[j] > tmax)
						penalty = -1;

					e = edge(i, j, *g);
					if (e.second && penalty >= 0){
						dist = distance(i, j);
						if (dist == 0)
							penalty = -1;
						else if (dist <= 5)
							penalty += (1 << (5 - dist)) * get(edge_weight_t(), *g, e.first);
						// else if (dist > 5) {}
					}
				}
			}
		}
	}

	return penalty;
}

/*
int Solution::calculatePenalty(int* indexvector, int nsub) {
	int i, j, dist;
	pair<G::Edge, bool> e;

	penalty = 0;
	for (i = 0; i < nsub && penalty >= 0; i++) {
		if (this->sol[i] < 1 || this->sol[i] > tmax)
			penalty = -1;

		for (j = i + 1; j < nsub && penalty >= 0; j++) {
			if (this->sol[j] < 1 || this->sol[j] > tmax)
				penalty = -1;

			e = edge(indexvector[i], indexvector[j], *this->g);
			if (e.second && penalty >= 0) {
				dist = distance(i, j);
				if (dist == 0)
					penalty = -1;
				else if (dist <= 5)
					penalty += (1 << (5 - dist)) * get(edge_weight_t(), *this->g, e.first);
				// else if (dist > 5) {}
			}
		}
	}

	return penalty;
}
*/

double Solution::calculatePenaltyFull(G::Graph* g, int studentNum) {
	return this->calculatePenalty(g) / double(studentNum);
}

int Solution::distance(int* sol, int i, int j) {
	int num1, num2;
	num1 = sol[i];
	num2 = sol[j];

	return num1 > num2 ? num1 - num2 : num2 - num1;
}

int Solution::distance(int i, int j) {
	int num1, num2;
	num1 = this->sol[i];
	num2 = this->sol[j];

	return num1 > num2 ? num1 - num2 : num2 - num1;
}

string Solution::printSolution() {
	string output = "";
	for (int i = 0; i < this->n; i++) {
		output += to_string(i + 1) + " " + to_string(sol[i]);
		if (i < (this->n - 1))
			output += "\n";
	}

	ofstream file(this->filename);
	file << output;
	file.close();
	return output;
}

string Solution::printSolution(std::string filename) {
	string output = "";
	for (int i = 0; i < this->n; i++) {
		output += to_string(i + 1) + " " + to_string(sol[i]);
		if (i < (this->n - 1))
			output += "\n";
	}

	ofstream file(filename);
	file << output;
	file.close();
	return output;
}

void Solution::checkSetPrintSolution(G::Graph* g, Solution* sol) {
	mtx.lock();
	if (this->penalty < 0 || sol->penalty < this->penalty) {
		this->setSolution(sol);
		this->calculatePenalty(g);
		this->printSolution();
		// std::cout << "Penalita' = " << this->penalty / (float)this->num_studenti << "\n";
	}
	mtx.unlock();
}














