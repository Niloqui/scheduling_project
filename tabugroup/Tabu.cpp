#include "Tabu.hpp"
#include "../kempegroup/Kempe.hpp"
#include "../LocalSearchGroup/LocalSearch.hpp"
#include <algorithm>
#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>
#include <deque>
#include <cmath>
#define PREDECESSOR -1
#define OBSOLETE LONG_MAX //Valore che indica che il valore non è più valido


using namespace std;

Tabu::Tabu(int teta, G::Graph& g, Solution& s, int studentNum):teta(teta),mu(0.6),exams(s.n),tmax(s.tmax),studentNum(studentNum){
    this->moveMatrix = new long*[exams];
    this->validityArray = new bool[exams]; //false se le mosse dell esame i val[i]
    //sono non aggiornate
    
    //Si presuppone che viene passata una soluzione già feasible
    colorGraph(g, s);

    //Ogni nodo ha ha tmax - 1 possibili mosse
    for(int i=0; i<exams; i++)
        this->moveMatrix[i] = new long[tmax+1];
        //+1 semplicemente per poter usare i veri nomi dei colori
        //senza dover stare ad aggiungere o sotrarre dopo
    
    //Le mosse inizialmente non sono valide
    for(int i=0; i<exams; i++){
        this->validityArray[i]=false;
        for(int j=1; j<=this->tmax; j++)
            this->moveMatrix[i][j] = OBSOLETE;
    }

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

void Tabu::swapColors(G::Graph& g){
    int iteratedColor;
    int color1 = rand() % this->tmax + 1;
    int color2 = rand() % this->tmax + 1;
            
    //Iterazione sui vertici
    G::Graph::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(g); v != vend; ++v){
        iteratedColor = get(vertex_color_t(),g,*v);
        if(iteratedColor == color1){//cambia a colore2
            put(vertex_color_t(),g,*v,color2);
            renderFalseMoves(g, *v);
        }
        else if(iteratedColor == color2){//cambia a colore 3
            put(vertex_color_t(),g,*v,color1);
            renderFalseMoves(g, *v);
        }
    }
    
    
    
}

/*Cerchiamo di scabiare colore con un kempe swap fra due nodi che appartengono
 a catene diverse se si considerano i colori del nodo 1 e del nodo 2*/
//tuple ritorna vertice1, vertice2, colore
std::tuple<long int,long int,int> Tabu::bestDoubleKempe(G::Graph& g){
    int i,j,x;
    int color1,color2;
    long penalty1, penalty2;
    long bestPenalty = INT_MAX; //affinche possa venir migliorata da qualsiasi swap valido
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
    long int penalty;
    v1 = vertex(id1, g);
    v2 = vertex(id2, g);
    
    //La penalita dev'essere calcolata prima dell'obsolescenza
    penalty = this->moveMatrix[id1][color] + this->moveMatrix[id2][color];
    
    tabuSimpleKempeWrapper(g, v1, color);
    tabuSimpleKempeWrapper(g, v2, color);

    return (int) penalty;
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
//Ma in quel caso pure i vertici adiacenti sono falsi
void Tabu::renderFalseMoves(G::Graph& g,G::Vertex v){
    long int id = get(vertex_index_t(),g,v);
    this->validityArray[id]=false;
    for(int color=1; color<=this->tmax; color++)
        this->moveMatrix[id][color] = OBSOLETE;
}

//Rende il vertice e quelli adiacenti falsi
void Tabu::renderFalseAndAdjacent(G::Graph& g, G::Vertex v){
    G::Graph::adjacency_iterator vit, vend;
    renderFalseMoves(g,v);
    for (boost::tie(vit, vend) = adjacent_vertices(v, g); vit != vend; ++vit){
        renderFalseMoves(g, *vit);
    }
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
    long int index = get(vertex_index_t(),g,v);

    //Modifico solo chi è obsoleto
    for(int color=1; color<=this->tmax; color++){
        if(this->moveMatrix[index][color]!=OBSOLETE)
            continue;
        
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
    long bestPenalty = this->moveMatrix[0][1];
    
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
static bool aspirationCriterion(int accumulatedPenalty, long movePenalty){
    return (0 > accumulatedPenalty + movePenalty);
}


//Copia una matrice in un 'altra, serve per tener conto delle mosse della miglior matrice
//finora trovate
static void matriscopy (long int** destmat, long int** srcmat,int ROWCOUNT,int COLUMNCOUNT)
{
    for(int i=0; i<ROWCOUNT; i++){
        for(int j=1; j<COLUMNCOUNT; j++){
            destmat[i][j] = srcmat[i][j];
        }
    }
}

static long** createMatrix(int ROWCOUNT,int COLUMNCOUNT){
    long int** newMatrix = new long*[ROWCOUNT];
    
    for(int j=0; j<ROWCOUNT; j++)
        newMatrix[j] = new long[COLUMNCOUNT];
    
    
    return newMatrix;
}

//Deve Venir Passato un grafo già valido, solution deve venir passato solo
//Per venire aggiornato
//Inoltre si intende che solution contiene la miglior soluzione globale
bool Tabu::tabuSearch(G::Graph& g, Solution& s,int maxNonImprovingIterations,int bestGlobalPenalty, TimeController& tlim, Solution& mothersolution){
    int initialTeta = this->teta;
    long** bestMatrix = createMatrix(this->exams, this->tmax+1);
    int bestMove=1; //miglior penalità ottenuta,
    long bestMovePenalty;
    G::Vertex bestVertex;
    bool better = false;
    int nonImprovingIterations = 0; //conta il numero di iterazioni senza miglioramenti
    Solution neighborhoodBestSolution(s.n,s.tmax);
    setSolution(g, neighborhoodBestSolution);
    
    //Correntemente la miglior matrice è quella di adesso
    matriscopy(bestMatrix, this->moveMatrix, this->exams, (this->tmax+1)); ///<-This somehow is MISTAKEN!!
    
    int neighborhoodBestPenalty = neighborhoodBestSolution.calculatePenalty(g);
    
    int accumulatedPenalty = 0; //penalità di base,resettata ad ogni soluzione
    //migliorante del vicinato accettata, quando questa giunge a valori negativi
    //vuol dire che quella mossa migliorerebbe la soluzione
    
    //Si fa una tabu search finche il numero massimo di iterazioni non miglioranti
    //è minore ad un certo parametro (maxNonImprovingIterations) passato
    double duration;
    while( nonImprovingIterations <= maxNonImprovingIterations && tlim.isThereTime() ){
        
        
        //Vediamo qual è la miglior mossa fra tutte
        //Bisogna tenere in considerazione la matrice delle mosse ed aggiornarla
        //Correttamente, in questa maniera i calcoli vengono significativamente ridotti
        std::pair<long int,int> pairMove = this->bestMove();
        bestVertex = vertex(pairMove.first,g);
        bestMove = pairMove.second;
        bestMovePenalty = this->moveMatrix[bestVertex][bestMove];
        //Fra tutte le mosse possibili effettuo la migliore
        tabuSimpleKempeWrapper(g, bestVertex, bestMove);
        /*else{
            std::tuple<long int,long int, int> bestMoveDouble = bestDoubleKempe(g);
            bestMovePenalty=doubleKempeSwap(g, get<0> (bestMoveDouble), get<1> (bestMoveDouble), get<2> (bestMoveDouble));
        }*/

        //Devono venire aggiornate le mosse
        updateMoveMatrix(g);
         
         //Zero diventa la nuova penalità accumulata a cui si sommano le mosse
         //Per vedere se si giunge all'aspiration criterion
         if(aspirationCriterion(accumulatedPenalty, bestMovePenalty)){
             accumulatedPenalty = 0; //nuova
             nonImprovingIterations = 0; //resettato il numero di iterazioni
             setSolution(g, neighborhoodBestSolution);
             neighborhoodBestPenalty = neighborhoodBestSolution.calculatePenalty(g);
             matriscopy(bestMatrix, this->moveMatrix, this->exams, (this->tmax+1));
             
             if(neighborhoodBestPenalty < bestGlobalPenalty)
             {
                 bestGlobalPenalty = neighborhoodBestPenalty;
                 colorGraph(g, neighborhoodBestSolution);
                 setSolution(g, s); //DO NOT DELETE
                 matriscopy(this->moveMatrix, bestMatrix, this->exams, (this->tmax+1));
                 
                 //s.printSolution(filename);
                 s.penalty = neighborhoodBestPenalty;
                 mothersolution.checkSetPrintSolution(&g, &s);
                 //cout << "Penalità: "  << s.calculatePenaltyFull(g, this->studentNum)<< endl;
                 better = true;
                
             }
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
    
    return better;
    
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
        //randomNode = rand() % q;
        randomNode = choiceprob(q, 1);
        //mi segno che la mossa è stata modificata, e i vertici adiacenti
        renderFalseAndAdjacent(g, vertex(randomNode,g));
        //Questa è la maniera in cui scegliamo il colore per rendere più probabile
        //ottenere i numeri con maggior penalità
        randomColor = rand() % tmax + 1; //Colore al quale cambiare
        randomVertex = vertex(randomNode,g);
        
        if(randomColor == get(vertex_color_t(),g,randomVertex))
            randomColor = ((randomColor + 1) % (tmax)) + 1;
        
        tabuSimpleKempeWrapper(g, randomVertex, randomColor);
    }
    
    
    delete [] idPenalty;
    return;
}


void Tabu::tabuIteratedLocalSearch(G::Graph& g, Solution& s,int tollerance, TimeController& tlim, Solution& mothersolution){
    int etamin = 4, etamax = 15;
    int nonImprovingTabus = 0;
    int q = min(30,s.n/3); //numero di nodi da perturbare
    int eta=etamin; //intensità della perturbazione
    bool improved=false;
    int bestGlobalPenalty = s.calculatePenalty(g);
    colorGraph(g, s);
    
    
    while( tlim.isThereTime() ){
        //L'intensità della perturbazione aumento più tabu ci sono
        //senza miglioramenti
        eta = min(etamin+nonImprovingTabus, etamax);
        improved = this->tabuSearch(g, s, tollerance, bestGlobalPenalty, tlim, mothersolution);
        
        if(improved){
            nonImprovingTabus = 0;
            bestGlobalPenalty = s.calculatePenalty(g);
        }
        else{
            nonImprovingTabus++;
        }
        
        //Dopo un numero tollerance di iterazioni del TS
        //si perturba il vicinato e si riparte con un nuovo TS
        
        tabuPerturbate(g, q, eta, s.tmax);
        
        //CODICE NON CONVINCENTE, DA RIMUOVERE SE FA SCHIFO
        int swapFrequency = 40;
        if(nonImprovingTabus%swapFrequency == 0 && nonImprovingTabus!=0){
            swapColors(g);
            //cout<<"Swapping colors"<<endl;
            nonImprovingTabus = 0;
        }
        
        updateMoveMatrix(g);
        //this->printMatrix();
        this->resetTabuList();
        
    }
}
