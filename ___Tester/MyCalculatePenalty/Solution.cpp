#include "Solution.hpp"

#include "graphw.hpp"



Solution::Solution(int n, int tmax) {
	this->n = n;
	this->tmax = tmax;
	sol = new int[n];

	for (int i = 0; i < n; i++)
		sol[i] = -1;
}

int Solution::calculatePenalty(G::Graph g) {
	int i, j, dist;
	std::pair<G::Edge, bool> e;

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

