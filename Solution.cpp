#include "Solution.hpp"

#include <fstream>
#include "graphw.hpp"
#include "Utility.hpp"

using namespace std;

Solution::Solution() {
	tmax = n = -1;
	sol = NULL;
}

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
	int i;
	
	this->n = sol->n;
	this->tmax = sol->tmax;
	this->optimum = sol->optimum;
	this->penalty = sol->penalty;
	this->mat = sol->mat;

	if (sol->indexexams == NULL) {
		this->indexexams == NULL;
	}
	else {
		this->indexexams = new int[sol->n];
		for (i = 0; i < this->n; i++)
			this->indexexams[i] = sol->indexexams[i];
	}

	this->sol = new int[sol->n];
	for (i = 0; i < this->n; i++)
		this->sol[i] = sol->sol[i];
}

void Solution::setSolution(Solution newsol, bool indexes) {
	if (indexes) {
		for (int i = 0; i < newsol.n; i++)
			this->sol[newsol.indexexams[i]] = newsol.sol[i];
	}
	else {
		for (int i = 0; i < this->n; i++)
			this->sol[i] = newsol.sol[i];
	}
}

void Solution::setSolution(Solution *newsol, bool indexes) {
	if (indexes) {
		for (int i = 0; i < newsol->n; i++)
			this->sol[newsol->indexexams[i]] = newsol->sol[i];
	}
	else {
		for (int i = 0; i < this->n; i++)
			this->sol[i] = newsol->sol[i];
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

int Solution::calculatePenalty() {
	int i, j, pen = 0, dist;

	for (i = 0; i < this->n && pen >= 0; i++) {
		if (this->sol[i] < 1 || this->sol[i] > tmax)
			pen = -1;

		for (j = i + 1; j < this->n && pen >= 0; j++) {
			if (this->sol[j] < 1 || this->sol[j] > tmax)
				pen = -1;

			if (mat[i][j] > 0) {
				dist = distance(i, j);
				if (dist == 0)
					pen = -1;
				else if (dist <= 5)
					pen += integerPower(2, 5 - dist) * mat[i][j];
			}
		}
	}

	return penalty = pen;
}

int Solution::calculatePenalty(G::Graph g) { // Usare adjacency_list per trovare i nodi adiacenti
	int i, j, pen = 0, dist;
	pair<G::Edge, bool> e;
	
	for (i = 0; i < this->n && pen >= 0; i++) {
		if (this->sol[i] < 1 || this->sol[i] > tmax)
			pen = -1;

		for (j = i+1; j < this->n && pen >= 0; j++) {
			if (this->sol[j] < 1 || this->sol[j] > tmax)
				pen = -1;

			e = getEdge(i, j, &g);
			if (e.second) {
				dist = distance(i, j);
				if (dist == 0)
					pen = -1;
				else if (dist <= 5)
					pen += integerPower(2, 5 - dist) * get(edge_weight_t(), g, e.first);
				// else if (dist > 5) {}
			}
		}
	}

	return penalty = pen;
}

/*
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
*/
int Solution::calculatePenalty(G::Graph g, int* indexvector, int nsub) {
	int i, j, pen = 0, dist;
	pair<G::Edge, bool> e;

	for (i = 0; i < nsub && pen >= 0; i++) {
		if (this->sol[i] < 1 || this->sol[i] > tmax)
			pen = -1;

		for (j = i + 1; j < nsub && pen >= 0; j++) {
			if (this->sol[j] < 1 || this->sol[j] > tmax)
				pen = -1;

			e = edge(indexvector[i], indexvector[j], g);
			if (e.second && pen >= 0) {
				dist = distance(i, j);
				if (dist == 0)
					pen = -1;
				else if (dist <= 5)
					pen += integerPower(2, 5 - dist) * get(edge_weight_t(), g, e.first);
				// else if (dist > 5) {}
			}
		}
	}

	return penalty = pen;
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
					if (e.second && pen >= 0){
						dist = distance(i, j);
						if (dist == 0)
							pen = -1;
						else if (dist <= 5)
							pen += integerPower(2, 5 - dist) * get(edge_weight_t(), g, e.first);
						// else if (dist > 5) {}
					}
				}
			}
		}
	}

	return penalty = pen;
}

int Solution::calculatePenalty(G::Graph g, int* sol, int* indexvector, int nsub, int tmax) {
	int i, j, pen = 0, dist;
	pair<G::Edge, bool> e;

	for (i = 0; i < nsub && pen >= 0; i++) {
		if (sol[i] < 1 || sol[i] > tmax)
			pen = -1;

		for (j = i + 1; j < nsub && pen >= 0; j++) {
			if (sol[j] < 1 || sol[j] > tmax)
				pen = -1;

			e = edge(indexvector[i], indexvector[j], g);
			if (e.second && pen>=0) {
				dist = Solution::distance(sol, i, j);
				if (dist == 0)
					pen = -1;
				else if (dist <= 5)
					pen += integerPower(2, 5 - dist) * get(edge_weight_t(), g, e.first);
				// else if (dist > 5) {}
			}
		}
	}

	return pen;
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

/*
string Solution::printSolution(ofstream file) {
	string output = "";
	for (int i = 0; i < this->n; i++) {
		output += to_string(i + 1) + " " + to_string(sol[i]);
		if (i < (this->n - 1))
			output += "\n";
	}

	file << output;
	return output;
}
*/

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


double Solution::calculatePenaltyFull(G::Graph g,int studentNum){
    int i, j, dist;
    double pen = 0;
    pair<G::Edge, bool> e;

    for (i = 0; i < this->n && pen >= 0; i++) {
        if (this->sol[i] < 1 || this->sol[i] > tmax)
            pen = -1;

        for (j = i+1; j < this->n && pen >= 0; j++) {
            if (this->sol[j] < 1 || this->sol[j] > tmax)
                pen = -1;

            e = getEdge(i, j, &g);
            if (e.second && pen >= 0) {
                dist = distance(i, j);
                if (dist == 0)
                    pen = -1;
                else if (dist <= 5)
                    pen += double(integerPower(2, 5 - dist)) * double(get(edge_weight_t(), g, e.first))/double(studentNum);
                // else if (dist > 5) {}
            }
        }
    }
	penalty = pen * studentNum;
    return pen;
}

pair<G::Edge, bool> Solution::getEdge(int i, int j, G::Graph *g) {
	if (this->indexexams != NULL)
		return edge(this->indexexams[i], this->indexexams[j], *g);
	else
		return edge(i, j, *g);
}

int** Solution::buildMatrix(G::Graph *g) {
	int i, j;
	std::pair<G::Edge, bool> e;

	mat = new int*[n];
	for (i = 0; i < n; i++)
		mat[i] = new int[n];

	for (i = 0; i < n; i++) {
		mat[i][i] = 0;

		for (j = i + 1; j < n; j++) {
			e = getEdge(i, j, g);

			if (e.second)
				mat[j][i] = mat[i][j] = get(edge_weight_t(), *g, e.first);
			else
				mat[j][i] = mat[i][j] = 0;
		}
	}

	return mat;
}















