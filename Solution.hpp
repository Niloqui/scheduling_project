#ifndef Solution_hpp
#define Solution_hpp

#include "graphw.hpp"
#include <fstream>
#include <string>

class Solution{
public:
	int* sol = NULL;
	/* L'indice di un elemento del vettore sol indica l'esame
	 * Gli esami sono sfasati di uno rispetto all'indice nel vettore
	 * Il valore di un elemento del vettore sol indica il suo time slot
	 * ex. sol[4] = 7; L'esame 5 (4 = 5 - 1) si trova nel time slot 7
	 * Il reader diminuisce l'esame di 1, quindi non � necessario eseguire la sottrazione scritta sopra per accedere al vettore
	 */
	int n = -1, tmax = -1, penalty = -1, studentNum;
	// n = lunghezza del vettore
	std::string filename; // File dove stampare il risultato. Utile solo per mothersolution

	Solution(); // Costruttore vuoto
	Solution(int n, int tmax); // Costruttore
	Solution(int n, int tmax, int* newsol); // Costruttore con soluzione iniziale
	Solution(Solution *sol); // Costruttore che copia da un'altra soluzione

	void setSolution(int* newsol);
	void setSolution(int* newsol, int* indexexams, int nsub);
	void setSolution(Solution* newsol);
	// Si assume che newsol abbia la stessa lunghezza di this->sol

	int calculatePenalty(G::Graph g);
	int calculatePenalty(G::Graph* g);
	int calculatePenalty(G::Graph* g, bool* mask);
	//int calculatePenalty(int* indexvector, int nsub);
	// Tutte le funzioni calculatePenalty restituiscono -1 se la soluzione � infeasible
	// La penalit� non viene divisa per il numero di studenti

	double calculatePenaltyFull(G::Graph* g, int studentNum);
	// Calcola la penalit� dividendo per il numero di studenti

	std::string printSolution();
	std::string printSolution(std::string filename);
	// std::string printSolution(std::ofstream file);
	// Esporta la soluzione su file e ritorna l'intero output che viene stampato a video

	// ~Solution() { delete[] sol; delete[] indexexams; }

	// Altro
	int distance(int i, int j);
	static int distance(int *sol, int i, int j);

	void checkSetPrintSolution(G::Graph* g, Solution* sol);
	//void checkSetPrintSolution(G::Graph* g, Solution* sol, int studentNum);
	// Questa funzione serve per scrivere in mothersolution
	// Inoltre esegue il controllo per capire se sol � migliore di this
};
	
	

#endif
