#include "Tabu.hpp"
#include "../kempegroup/Kempe.hpp"
#include "../LocalSearchGroup/LocalSearch.hpp"
#include <algorithm>
#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <deque>
#include <cmath>
#define PREDECESSOR -1


using namespace std;

Tabu::Tabu(int teta,G::Graph& g,Solution& s):teta(teta),mu(0.6),exams(s.n),tmax(s.tmax){
    this->moveMatrix = new int*[exams];
    this->validityArray = new bool[exams]; //false se le mosse dell esame i val[i]
    //sono non aggiornate
    
    //Si presuppone che viene passata una soluzione già feasible
    colorGraph(g, s);
    
    //Ogni nodo ha ha tmax - 1 possibili mosse
    for(int i=0; i<exams; i++)
        this->moveMatrix[i] = new int[tmax+1];
        //+1 semplicemente per poter usare i veri nomi dei colori
        //senza dover stare ad aggiungere o sotrarre dopo
    
    //Le mosse inizialmente non sono validit
    for(int i=0; i<exams; i++)
        this->validityArray[i] = false;
    
    //Calcolo delle possibili mosse
    G::Graph::vertex_iterator v, vend;
    long int iteratedId;
    
    //Calcoliamo il costo per le mosse adesso
    for (boost::tie(v, vend) = vertices(g); v != vend; ++v) {
        iteratedId = get(vertex_index_t(),g,*v);
        updateNodePenalties(g, *v);
        this->validityArray[iteratedId] = true;
    }
      
    
}

/*Cerchiamo di scabiare colore con un kempe swap fra due nodi che appartengono
 a catene diverse se si considerano i colori del nodo 1 e del nodo 2*/
//tuple ritorna vertice1, vertice2, colore
std::tuple<long int,long int,int> Tabu::bestDoubleKempe(G::Graph& g){
    int i,j,x;
    int color1,color2;
    int penalty1, penalty2;
    int bestPenalty = INT_MAX; //affinche possa venir migliorata da qualsiasi swap valido
    std::tuple<long int,long int,int> bestTuple;
    
    for(i=0; i<this->exams-1; i++){
        color1 = get(vertex_color_t(),g,vertex(i, g));
        
        for(j=i+1; j<this->exams ; j++){
            color2 = get(vertex_color_t(),g,vertex(j, g));
            
            if(color1!= color2)
                continue;
            
            //Cerco le possibili mosse purch+ siano appartenenti a diverse catene
            for(x=1; x<=this->tmax; x++){
                penalty1 = this->moveMatrix[i][x];
                penalty2 = this->moveMatrix[j][x];
                //Se questa condizione è vera allora appertengono alla stessa catena
                if( penalty1 == penalty2 || isTabu(i, x) || isTabu(j, x))
                    continue;
                else if(penalty2+penalty1< bestPenalty){
                    bestPenalty = penalty1+penalty2;
                    get<0> (bestTuple) =i;
                    get<1> (bestTuple) =j;
                    get<2> (bestTuple) = x; //miglior colore
                }
            }

        }
    }
    
    return bestTuple;
}

int Tabu::doubleKempeSwap(G::Graph& g,long int id1,long int id2, int color){
    G::Vertex v1,v2;
    int penalty;
    v1 = vertex(id1, g);
    v2 = vertex(id2, g);
    
    tabuSimpleKempeWrapper(g, v1, color);
    tabuSimpleKempeWrapper(g, v2, color);

    penalty = this->moveMatrix[id1][color] + this->moveMatrix[id2][color];
    return penalty;
}

std::pair<long int,long int> Tabu::fastBestDouble(G::Graph& g){
    pair<long int,long int> bestSwap;
    int c1,c2;
    int penalty1,penalty2;
    int bestPenalty = INT_MAX;
    
    for(int i=0; i<this->exams-1; i++){
        c1 = get(vertex_color_t(),g,vertex(i,g));
        for(int j=i+1; j<this->exams; j++){
            c2 = get(vertex_color_t(),g,vertex(j,g));
            
            penalty1 = this->moveMatrix[i][c2];
            penalty2 = this->moveMatrix[j][c1];
            
            
            if(c1==c2 || penalty1==penalty2 || isTabu(i, c2) || isTabu(j, c1))
                continue;
            
            if(bestPenalty>penalty1+penalty2){
                bestSwap.first = i;
                bestSwap.second= j;
                bestPenalty = penalty2+penalty1;
            }
        }
    }
    
    return bestSwap;
}

int Tabu::fastDoubleKempeSwap(G::Graph& g, long int id1, long int id2){
    G::Vertex v1,v2;
    int c1,c2;
    
    v1 = vertex(id1,g);
    v2 = vertex(id2,g);
    c1 = get(vertex_color_t(),g,v1);
    c2 = get(vertex_color_t(),g,v2);
    
    tabuSimpleKempeWrapper(g, v1, c2);
    tabuSimpleKempeWrapper(g, v2, c1);
    
    int penalty = this->moveMatrix[v1][c2] + this->moveMatrix[v2][c1];
    return penalty;
}

void Tabu::printMatrix(){
    int i,j;
    
    for(i=0; i<this->exams; i++){
        for(j=1; j<=this->tmax;j++){
            cout << this->moveMatrix[i][j] << " ";
        }
        
        cout << endl;
    }
    
    cout << endl;
}

//Si fa lo scan della matrice e si aggiornano quelle mosse segnate come false
//In realta quando un nodo viene cambiato tutte le sue mosse sono false
void Tabu::updateMoveMatrix(G::Graph& g){
    G::Vertex vit;
    
    for(int id=0; id<this->exams; id++){
        if(this->validityArray[id]==false){
            vit = vertex(id,g);
            updateNodePenalties(g, vit);
        }
    }
}

//Tutte le mosse associate a v sono false e vengono segnate come non aggiornate
void Tabu::renderFalseMoves(G::Graph& g,G::Vertex v){
    long int id = get(vertex_index_t(),g,v);
    this->validityArray[id]=false;
}


//Calcolatore delle penalità associate ad una mossa
int Tabu::tabuKempeMovePenalty(G::Graph& g, G::Vertex v, int color,  std::unordered_set<long int>& visitedNodes){
    int penalty = 0;
    //Colore originale da resettare alla fine della funzione
    int originalColor = get(vertex_color_t(),g,v);
    int iteratedColor;
    long int iteratedId;
    
    //Memorizing that the current node has been visited
    visitedNodes.insert(get(vertex_index_t(),g,v));
    
    penalty += nodeMovePenalty(g, v, color);
    
    //Nuovamente, vogliamo evitare di ciclare infinitamente
    put(vertex_color_t(),g,v,PREDECESSOR);
    
    G::Graph::adjacency_iterator vit, vend;
    for (boost::tie(vit, vend) = adjacent_vertices(v, g); vit != vend; ++vit) {
        //Cerco il colore relativo alla potenziale mossa da effettuare
        iteratedColor = get(vertex_color_t(),g,*vit);
        iteratedId = get(vertex_index_t(),g,*vit);
        if (iteratedColor == color && visitedNodes.count(iteratedId) == 0 ){
            penalty += tabuKempeMovePenalty(g, *vit, originalColor,visitedNodes);
        }
    }

    
    put(vertex_color_t(),g,v,originalColor);
    return penalty;
}


//Questa funzione aggiorna i costi relativi alle penalità di una certa kempe chain qualora si spostasse in una certa direzione
//Segna i valori nella matrice della classe tabu relativa alle penalita
//Color indica il colore verso il quale mi sposto
//quindi è più che altro una funzione di supporto, che mi aiuta a propagare i cambiamenti
void Tabu::updatePenalty(G::Graph& g, G::Vertex v, int color, int penalty,std::unordered_set<long int>& visitedNodes){
    int iteratedColor;
    int myColor = get(vertex_color_t(),g,v);
    long int iteratedId;
    //Segno il corrente vertice come già visitato
    long int myId =get(vertex_index_t(),g,v);
     visitedNodes.insert(myId);
    
    //aggiorno le informazioni per la mia penalità
    this->moveMatrix[myId][color] = penalty;

    G::Graph::adjacency_iterator vit, vend;
    for (boost::tie(vit, vend) = adjacent_vertices(v, g); vit != vend; ++vit){
        iteratedColor = get(vertex_color_t(),g,*vit);
        iteratedId = get(vertex_index_t(),g,*vit);
        if(iteratedColor == color && visitedNodes.count(iteratedId) == 0){
            updatePenalty(g, *vit, myColor, penalty, visitedNodes);
        }
    }
    
}

void Tabu::updatePenaltyWrapper(G::Graph& g, G::Vertex v, int color, int penalty){
    std::unordered_set<long int>visitedNodes;
    updatePenalty(g, v, color, penalty, visitedNodes);
}

//Aggiorna tutte le mosse di un nodo
//Mentre le funzioni di arriva aggiornano le mosse relative solo ad un certo colore
void Tabu::updateNodePenalties(G::Graph& g,G::Vertex v){
    int penalty;

    for(int color=1; color<=this->tmax; color++){
        penalty = tabuKempeMovePenaltyWrapper(g, v, color);
        updatePenaltyWrapper(g, v, color, penalty);
    }
    //this->printMatrix();
    this->validityArray[get(vertex_index_t(),g,v)]=true;
    
}

//Ho bisogno di un kempeMovePenalty che mi segni le correnti mosse esplorate
int Tabu::tabuKempeMovePenaltyWrapper(G::Graph& g, G::Vertex v, int color){
    std::unordered_set<long int> visitedNodes;
    int answer = tabuKempeMovePenalty(g, v, color,visitedNodes);
    return answer;
}

//Ritorna la miglior mossa correntemente disponibile
std::pair<long int,int> Tabu::bestMove(){
    int i,j;
    long int id = 0;
    int color = 1;
    int bestPenalty = this->moveMatrix[0][1];
    
    for(i=0; i<this->exams; i++){
        for(j=1; j<=this->tmax; j++){
            if(bestPenalty > this->moveMatrix[i][j] && !isTabu(i, j)){
                id = i;
                color = j;
                bestPenalty = this->moveMatrix[i][j];
            }
        }
    }
    
    return make_pair(id, color);
}




void Tabu::resetTabuList(){
    this->tabuList.clear();
}

//Cerca la lista Tabu per vedere se una mossa è proibita
bool Tabu::isTabu(long int id, int color){
    for (auto i = this->tabuList.begin(); i != this->tabuList.end(); ++i){
        if(id == (*i).first && color == (*i).second)
            return true;
    }
    return false;
}

bool Tabu::isTabu(G::Graph& g, G::Vertex v, int color){
    return isTabu(get(vertex_index_t(),g,v), color);
}

//Inserire mossa nella tabu list
//Elimina automaticamente mosse che ingrandiscano la lista oltre teta

void Tabu::insertMove(long int id,int color){
    pair<long int,int> move = make_pair(id, color);
    
    if(this->tabuList.size() >= this->teta)
        this->tabuList.resize(this->teta);
    
    this->tabuList.push_front(move);
}

//Overload della funzione di sopra
void Tabu::insertMove(G::Vertex v, G::Graph& g,int color){
    insertMove(get(vertex_index_t(),g,v), color);
}


//Simple Kempe applicando la tabu list
void Tabu::tabuSimpleKempe(G::Graph& g ,G::Vertex v, int color, std::unordered_set<long int>& visitedNodes){
    
    int myColor = get(vertex_color_t(),g,v);
    long int iteratedId;
    
    //Segnare come vertice visitato
    visitedNodes.insert(get(vertex_index_t(),g,v));
    
    //Bisogna iterare sui vertici adiacenti
    //Ritorna l'iterator range
    //G::Graph::adjacency_iterator iterator =  adjacent_vertices(v, g);
    
    //Per evitare i cicli, e quindi una ricorsione infinita, nei vertici già
    //Visitati si segna un colore fittizio -1 che non esiste, in questo modo
    //Nei cicli si evita di rivisitare sempre lo stesso cammino
    put(vertex_color_t(),g,v,PREDECESSOR);
    G::Graph::adjacency_iterator vit, vend;
    for (boost::tie(vit, vend) = adjacent_vertices(v, g); vit != vend; ++vit) {
        iteratedId = get(vertex_index_t(),g,*vit);
        
        //Anche i nodi adiacenti vengono resi falsi nei loro movimenti
        //Bisogna aggiornare anche questi
        this->renderFalseMoves(g, *vit);
        
        //Cerco il colore relativo allo swap desiderato
        if (get(vertex_color_t(),g,*vit) == color &&
            visitedNodes.count(iteratedId) == 0){
            tabuSimpleKempe(g,*vit,myColor,visitedNodes);
        }
    }
    
    put(vertex_color_t(),g,v,color);
    //Correntemente le mosse per questo array sono false, dovranno venire aggiornate
    this->renderFalseMoves(g, v);
    this->insertMove(v, g, myColor); //Mossa che di adesso in poi sarà proibita
    return;
    
}

void Tabu::tabuSimpleKempeWrapper(G::Graph& g ,G::Vertex v, int color){
    std::unordered_set<long int> visitedNodes;
    tabuSimpleKempe(g, v, color,visitedNodes);
    return;
    
}

//Retruns true if the aspiration criterion is satisfied, aka
//That move would make the solution in the CURRENT NEIGHTBORHOOD better;
static bool aspirationCriterion(int accumulatedPenalty, int movePenalty){
    return (0 > accumulatedPenalty + movePenalty);
}


//Copia una matrice in un 'altra, serve per tener conto delle mosse della miglior matrice
//finora trovate
static void matriscopy (int** destmat, int** srcmat,int ROWCOUNT,int COLUMNCOUNT)
{
    for(int i=0; i<ROWCOUNT; i++){
        for(int j=1; j<=COLUMNCOUNT; j++){
            destmat[i][j] = srcmat[i][j];
        }
    }
}

static int** createMatrix(int ROWCOUNT,int COLUMNCOUNT){
    int** newMatrix = new int*[ROWCOUNT];
    
    for(int j=0; j<ROWCOUNT; j++)
        newMatrix[j] = new int[COLUMNCOUNT];
    
    
    return newMatrix;
}

//Deve Venir Passato un grafo già valido, solution deve venir passato solo
//Per venire aggiornato
//Inoltre si intende che solution contiene la miglior soluzione globale
bool Tabu::tabuSearch(G::Graph& g, Solution& s,int maxNonImprovingIterations,int bestGlobalPenalty,clock_t start,int tlim){
    clock_t current = clock();
    int initialTeta = this->teta;
    int** bestMatrix = createMatrix(this->exams, this->tmax+1);
    double margin =  2;
    int bestMove=1,bestMovePenalty; //miglior penalità ottenuta,
    G::Vertex bestVertex;
    int nonImprovingIterations = 0; //conta il numero di iterazioni senza miglioramenti
    Solution neighborhoodBestSolution(s.n,s.tmax);
    setSolution(g, neighborhoodBestSolution);
    
    //Correntemente la miglior matrice è quella di adesso
    matriscopy(bestMatrix, this->moveMatrix, this->exams, (this->tmax+1));
    
    int neighborhoodBestPenalty = neighborhoodBestSolution.calculatePenalty(g);
    
    int accumulatedPenalty = 0; //penalità di base,resettata ad ogni soluzione
    //migliorante del vicinato accettata, quando questa giunge a valori negativi
    //vuol dire che quella mossa migliorerebbe la soluzione
    
    //Si fa una tabu search finche il numero massimo di iterazioni non miglioranti
    //è minore ad un certo parametro (maxNonImprovingIterations) passato
    while(nonImprovingIterations <= maxNonImprovingIterations &&
          ((double)clock() - (double)start)/CLOCKS_PER_SEC + margin < tlim ){
        
        
        //Applichiamo il simple kempe se il numero di esami è minore al numero di
        //Slot disponibili in quanto in questo caso il simple kempe, per come è implementato
        //Garantisce una migliore esplorazione dello spazio, in quanto basa i cambiamenti
        //sui colori tmax e non sui colori degli altri nodi
        //nel caso di exams>tmax questo problema non c'è in quanto lo squeaky wheel
        //Si ferma alla prima soluzione fattibile, e quindi ci è garantito che si usano
        //tutti i colori almeno una volta e quindi il double kempe dovrebbe essere
        //ottimale
        if(this->exams < this->tmax){
            //Vediamo qual è la miglior mossa fra tutte
            //Bisogna tenere in considerazione la matrice delle mosse ed aggiornarla
            //Correttamente, in questa maniera i calcoli vengono significativamente ridotti
            std::pair<long int,int> pairMove = this->bestMove();
            bestVertex = vertex(pairMove.first,g);
            bestMove = pairMove.second;
                bestMovePenalty = this->moveMatrix[bestVertex][bestMove];

            //Fra tutte le mosse possibili effettuo la migliore
            tabuSimpleKempeWrapper(g, bestVertex, bestMove);
        }
        else{
            std::tuple<long int,long int,int> pairDoubleMove = this->bestDoubleKempe(g);
            bestMovePenalty= doubleKempeSwap(g,get<0> (pairDoubleMove), get<1> (pairDoubleMove), get <2> (pairDoubleMove));
            //Dobbiamo anche ritornare best move penalty
        }
        
        //Devono venire aggiornate le mosse
        updateMoveMatrix(g);
         
         //Zero diventa la nuova penalità accumulata a cui si sommano le mosse
         //Per vedere se si giunge all'aspiration criterion
         if(aspirationCriterion(accumulatedPenalty, bestMovePenalty)){
             accumulatedPenalty = 0; //nuova
             nonImprovingIterations = 0; //resettato il numero di iterazioni
             //senza miglioramenti
             setSolution(g, neighborhoodBestSolution);
             neighborhoodBestPenalty = neighborhoodBestSolution.calculatePenalty(g);
             matriscopy(bestMatrix, this->moveMatrix, this->exams, (this->tmax+1));
             //cout << "Miglioramento ottenuto!" <<endl;
         }
         else{
             accumulatedPenalty += bestMovePenalty;
             nonImprovingIterations ++;
             
             //Aumentare la profondita della tabu list se siamo vicini
             //Ad una soluzione ottima globale
             if(accumulatedPenalty - 2*s.n < 0)
                 this->teta = (1+this->mu)*this->teta;
         }
        
    }
    
    this->teta = initialTeta;
    
    //Conviene creare un'altra soluzione per il neighborhood, e poi
    //Alla fine utilizzare setSolution solo se la soluzione migliore del
    //corrente vicinato è migliore di quella trovata fino ad adesso
    if(neighborhoodBestPenalty < bestGlobalPenalty)
    {   colorGraph(g, neighborhoodBestSolution);
        setSolution(g, s);
        
        matriscopy(this->moveMatrix, bestMatrix, this->exams, (this->tmax+1));
        
        return true;
    }
    
    return false;
    
}

void Tabu::tabuPerturbate(G::Graph& g, int q,int eta, int tmax){
    long int iteratedId;
    
    //Il primo elemento è l'id, il secondo è la penalità associata
    long int n = num_vertices(g);
    pair<long int,int>* idPenalty  = new pair<long int,int>[n];
    
    if(q>=n){
        cout << "ERROR: Please input a q smaller than the number of vertices" << endl;
        return;
    }
    
    //Bisogna iterare sui vertici
    G::Graph::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(g); v != vend; ++v) {
        iteratedId = get(vertex_index_t(),g,*v);

        idPenalty[iteratedId].first = iteratedId;
        idPenalty[iteratedId].second = nodeCurrentPenalty(g, *v);
    }
    
    //Vertici posti in ordine discendente per penalità
    sort(idPenalty, idPenalty + n, compare);
    
    //Adesso si effettuano eta mosse randomiche sui q più grandi
    //Effettiamo eta mosse randomiche
     srand (int(time(NULL))); //Seed randomica
    int randomNode;
    int randomColor;
    G::Vertex randomVertex;
    for(int j=0; j<eta; j++){
        //Index che sceglie uno dei q più grandi
        randomNode = rand() % q;
        //mi segno che la mossa è stata modificata
        renderFalseMoves(g, vertex(randomNode,g));
        //Questa è la maniera in cui scegliamo il colore per rendere più probabile
        //ottenere i numeri con maggior penalità
        randomColor = rand() % tmax + 1; //Colore al quale cambiare
        randomVertex = vertex(randomNode,g);
        
        if(randomColor == get(vertex_color_t(),g,randomVertex))
            randomColor = ((randomColor + 1) % (tmax)) + 1;
        
        simpleKempeWrapper(g, randomVertex, randomColor);
    }
    
    
    delete [] idPenalty;
    return;
}


void Tabu::tabuIteratedLocalSearch(G::Graph& g, Solution& s,int tollerance,clock_t start,int tlim){
    clock_t current = clock();
    double margin = 2;
    int etamin = 15, etamax = 100;
    int nonImprovingTabus = 5;
    int q = min(30,s.n/3); //numero di nodi da perturbare
    int eta=etamin; //intensità della perturbazione
    bool improved=false;
    int bestGlobalPenalty = s.calculatePenalty(g);
    colorGraph(g, s);
    
    
    while(((double)clock() - (double)start)/CLOCKS_PER_SEC + margin < tlim){
        //L'intensità della perturbazione aumento più tabu ci sono
        //senza miglioramenti
        eta = min(etamin+nonImprovingTabus, etamax);
        improved = this->tabuSearch(g, s, tollerance, bestGlobalPenalty,start,tlim);
        
        if(improved){
            nonImprovingTabus = 0;
            bestGlobalPenalty = s.calculatePenalty(g);
        }
        else
            nonImprovingTabus++;
            
        
        //Dopo un numero tollerance di iterazioni del TS
        //si perturba il vicinato e si riparte con un nuovo TS
        //cout << "Perturbating.."<< endl;
        tabuPerturbate(g, q, eta, s.tmax);
        updateMoveMatrix(g);
        //this->printMatrix();
        this->resetTabuList();
        
    }
}
