#ifndef Solution_hpp
#define Solution_hpp

#include "graphw.hpp"
#include <fstream>

class Solution{
public:
	int* sol;
	/* L'indice di un elemento del vettore sol indica l'esame
	 * Gli esami sono sfasati di uno rispetto all'indice nel vettore
	 * Il valore di un elemento del vettore sol indica il suo time slot
	 * ex. sol[4] = 7; L'esame 5 (4 = 5 - 1) si trova nel time slot 7
	 * Il reader diminuisce l'esame di 1, quindi non è necessario eseguire la sottrazione scritta sopra per accedere al vettore
	 */
	int n, tmax, penalty = -1;
	// n = lunghezza del vettore
	int* indexexams = NULL;
	// indexexams può essere utilizzato in un sottografo per contenere la soluzione di quel sottografo
	bool optimum = false;
	// Questa variabile ha più senso per le soluzioni ai sottografi che all'intero grafo
	// Se la dimensione del sottografo è piccola (circa 6/7 nodi), è possibile eseguire un algoritmo esatto
	// La sottosoluzione trovata da un algoritmo esatto avrà optimum = true
	// In caso di optimum = true, non si eseguiranno nuovi algoritmi per tentare di migliorare la soluzione trovata

	Solution(); // Costruttore vuoto
	Solution(int n, int tmax); // Costruttore
	Solution(int n, int tmax, int* newsol); // Costruttore con soluzione iniziale

	void setSolution(Solution newsol, bool indexes);
	// indexes=true => Si copieranno solo i valori negli indici del del vettore indexexams di newsol
	void setSolution(int* newsol);
	void setSolution(int* newsol, bool* mask);
	void setSolution(int* newsol, int* indexexams, int nsub);

	int calculatePenalty(G::Graph g);
	int calculatePenalty(G::Graph g, bool* mask);
	int calculatePenalty(G::Graph g, int* indexvector, int nsub);
	static int calculatePenalty(G::Graph g, int *sol, int* indexvector, int nsub, int tmax);
	// Tutte le funzioni calculatePenalty restituiscono -1 se la soluzione è infeasible
	// La penalità non viene divisa per il numero di studenti
    
    //Calcola la penalitˆ dividendo per il numero di studenti
    double calculatePenaltyFull(G::Graph g,int studentNum);
    
	std::string printSolution(std::string filename);
	// std::string printSolution(std::ofstream file);
	// Esporta la soluzione su file e ritorna l'intero output che viene stampato a video

	~Solution() { delete[] sol; delete[] indexexams; }

	// Altro
	int distance(int i, int j);
	static int distance(int *sol, int i, int j);



};
	
	

#endif
