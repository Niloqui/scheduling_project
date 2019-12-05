#ifndef Solution_hpp
#define Solution_hpp

#include "graphw.hpp"


class Solution{
private:
	int distance(int i, int j);

public:
	int* sol;
	/* L'indice di un elemento del vettore sol indica l'esame
	 * Gli esami sono sfasati di uno rispetto all'indice nel vettore
	 * Il valore di un elemento del vettore sol indica il suo time slot
	 * ex. sol[4] = 7; L'esame 5 (4 = 5 - 1) si trova nel time slot 7
	 * Il reader diminuisce l'esame di 1, quindi non è necessario eseguire la sottrazione scritta sopra per accedere al vettore
	 */
	int n, tmax;
	// n = lunghezza del vettore
	int* indexexams;
	bool* mask;
	// indexexams e mask possono essere utilizzati in un sottografo per contenere la soluzione di quel sottografo
	bool optimum;
	// Questa variabile ha più senso per le soluzioni ai sottografi che all'intero grafo
	// Se la dimensione del sottografo è piccola (circa 6/7 nodi), è possibile eseguire un algoritmo esatto
	// La sottosoluzione trovata da un algoritmo esatto avrà optimum = true
	// In caso di optimum = true, non si eseguiranno nuovi algoritmi per tentare di migliorare la soluzione trovata

	Solution(int n, int tmax); // Costruttore
	Solution(int n, int tmax, int* newsol); // Costruttore con soluzione iniziale

	void setSolution(Solution newsol, char car);
	// car = 'a' -> all; 'm' -> mask; 'i' -> index
	void setSolution(int* newsol);
	void setSolution(int* newsol, bool* mask);
	void setSolution(int* newsol, int* indexexams, int nsub);

	int calculatePenalty(G::Graph g);
	int calculatePenalty(G::Graph g, bool* mask);
	int calculatePenalty(G::Graph g, int* indexvector, int nsub);
	// Tutte le funzioni calculatePenalty restituiscono -1 se la soluzione è unfeasible

	// (forse) TO-DO: provare a invertire la seconda e la terza calculatePenalty, ovvero la terza sarà un wrapper della seconda.
	// Controllare poi quale delle due versioni è più efficiente

	void printSolution(std::string filename); // Esporta la soluzione su file

	~Solution() { delete[] sol; delete[] mask; delete[] indexexams; }


};
	
	

#endif
