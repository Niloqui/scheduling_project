#ifndef Tabu_hpp
#define Tabu_hpp

#include "../Solution.hpp"
#include "../TimeController.hpp"

class Tabu{
public:
    Tabu(int teta, G::Graph& g, Solution& s, int studentNum); //Accetta teta, ovvero la grandezza della tabu list
    void tabuIteratedLocalSearch(G::Graph& g, Solution& s, int tollerance, TimeController& tlim, Solution& mothersolution);
    //Max non improving iterations da il numero di mosse massime senza miglioramenti
    //Che sono permesse prima che il tabu search venga perturbato e subentri la
    //fase di iterated local search
    // ritorna True se il vicinato ha trovato un ottimo globale
    bool tabuSearch(G::Graph& g, Solution& s,int maxNonImprovingIterations, int bestGlobalPenalty, TimeController& tlim, Solution& mothersolution);
    void printMatrix();
    
private:

    /*FUNZIONI**/
    
    //Cerca la lista Tabu per vedere se una mossa è proibita
    bool isTabu(int id, int color);
    bool isTabu(G::Graph& g,G::Vertex v, int color);
    
    //Inserire mossa nella tabu list
    void insertMove(G::Vertex v, G::Graph& g,int color);
    void insertMove(long int id,int color);
    
    //
    void tabuSimpleKempeWrapper(G::Graph& g ,G::Vertex v, int color);
    void tabuSimpleKempe(G::Graph& g ,G::Vertex v, int color,std::unordered_set<int>& visitedNodes);
    void resetTabuList();
    
    //Si segna le mosse già esplorate in una matrice apposita
    int tabuKempeMovePenaltyWrapper(G::Graph& g, G::Vertex v, int color);
    int tabuKempeMovePenalty(G::Graph& g, G::Vertex v, int color,  std::unordered_set<int>& visitedNodes);
    void updatePenaltyWrapper(G::Graph& g, G::Vertex v, int color, int penalty);
    void updatePenalty(G::Graph& g, G::Vertex v, int color, int penalty,std::unordered_set<int>& visitedNodes);
    void updateMoveMatrix(G::Graph& g); //Aggiorna la matrice delle mosse
    void renderFalseMoves(G::Graph& g,G::Vertex v); //Renders all moves associated with
    //node v false
    void renderFalseAndAdjacent(G::Graph& g, G::Vertex v);//REnde il vertice e
    //Adiacenti falsi
    
    //Aggiorna le penalita di un nodo
    void updateNodePenalties(G::Graph& g,G::Vertex v);
    std::pair<int,int> bestMove();
    
    //Perturba ma anche si ricorda delle mosse precalcolate
    void tabuPerturbate(G::Graph& g, int q,int eta, int tmax);
    
    //Ritorna i due nodi i cui colori swappare,
    //La scelta è fatta in maniera tale che questi due nodi appartengono a due catene
    //Kempe diverse, per cui si effettua un doppio kempe
    //tuple ritorna nodo1,nodo2, colore
    std::tuple<int,int,int> bestDoubleKempe(G::Graph& g);
    
   //Accetta due nodi e il colore al quale devono venir swappati
    int doubleKempeSwap(G::Graph&,int,int,int);
    
    //Swappa due colori randomici
    void swapColors(G::Graph& g);
    
    /*-----DATI---**/
    long int** moveMatrix;
    bool* validityArray;
    //l'intero ci dice quanto la mossa costi verso quel colore, il booleano ci dice
    //Se la mossa è aggiornata e valida
    double mu; //moltiplicatore per la profondità della tabu list
    int tmax;
    int exams;
    int teta; //Lunghezza della tabu list
    //Lista delle mosse proibite, il primo intero è l'id del vettore
    //Il secondo elemento è il colore al quale non si può muovere
    std::deque<std::pair<int,int>> tabuList;
    int studentNum;
    
};


#endif /* Tabu_hpp */
