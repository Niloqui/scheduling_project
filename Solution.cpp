#include "Solution.hpp"


#include <stdio.h>
#include <iostream>
#include "graphw.hpp"
#include "Utility.hpp"


using namespace std;


Solution::Solution(int n, int tmax) {
	this->n = n;
	this->tmax = tmax;
	this->sol = new int[n];

	for (int i = 0; i < n; i++)
		this->sol[i] = -1;
}

Solution::Solution(int n, int tmax, int* newsol) {
	this->n = n;
	this->tmax = tmax;
	this->sol = new int[n];

	setSolution(newsol);
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

void Solution::setSolution(int* newsol, int* indexvector, int nsub) {
	for (int i = 0; i < nsub; i++)
		this->sol[indexvector[i]] = newsol[i];
}

int Solution::calculatePenalty(G::Graph g){
	int i, j, pen = 0, dist;
	pair<G::Edge, bool> e;

	for (i = 0; i < this->n && pen >= 0; i++) {
		if (this->sol[i] < 0 || this->sol[i] >= tmax)
			pen = -1;

		for (j = i+1; j < this->n && pen >= 0; j++) {
			if (this->sol[j] < 0 || this->sol[j] >= tmax)
				pen = -1;

			e = edge(i, j, g);
			if (e.second) {
				dist = distance(i, j);
				if (dist == 0)
					pen = -1;
				else if (dist <= 5)
					pen += integerPower(2, dist) * get(edge_weight_t(), g, e.first);
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
		mask[indexvector[1]] = true;

	pen = calculatePenalty(g, mask);
	delete[] mask;
	return pen;
}

int Solution::calculatePenalty(G::Graph g, bool* mask) {
	int i, j, pen = 0, dist;
	pair<G::Edge, bool> e;

	for (i = 0; i < this->n && pen>=0; i++) {
		if (mask[i]) {
			if (this->sol[i] < 0 || this->sol[i] >= tmax)
				pen = -1;

			for (j = i+1; j < this->n && pen >= 0; j++) {
				if (mask[j]){
					if (this->sol[j] < 0 || this->sol[j] >= tmax)
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





















