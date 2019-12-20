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

Tabu::Tabu(int teta):teta(teta),mu(0.6){}

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
        
        //Cerco il colore relativo allo swap desiderato
        if (get(vertex_color_t(),g,*vit) == color &&
            visitedNodes.count(iteratedId) == 0){
            tabuSimpleKempe(g,*vit,myColor,visitedNodes);
        }
    }
    
    put(vertex_color_t(),g,v,color);
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

//Deve Venir Passato un grafo già valido, solution deve venir passato solo
//Per venire aggiornato
//Inoltre si intende che solution contiene la miglior soluzione globale
bool Tabu::tabuSearch(G::Graph& g, Solution& s,int maxNonImprovingIterations,int bestGlobalPenalty,clock_t start,int tlim){
    clock_t current = clock();
    int initialTeta = this->teta;
    double margin =  2*((double)current-(double)start)/CLOCKS_PER_SEC;
    int maxColor = s.tmax;
    int color,iteratedColor;
    int bestMove=1,bestMovePenalty=0; //miglior penalità ottenuta,
    G::Vertex bestVertex;
    int penalty; //penalita della mossa corrente
    long int iteratedId;
    int nonImprovingIterations = 0; //conta il numero di iterazioni senza miglioramenti
    Solution neighborhoodBestSolution(s.n,s.tmax);
    setSolution(g, neighborhoodBestSolution);
    
    int neighborhoodBestPenalty = neighborhoodBestSolution.calculatePenalty(g);
    
    int accumulatedPenalty = 0; //penalità di base,resettata ad ogni soluzione
    //migliorante del vicinato accettata, quando questa giunge a valori negativi
    //vuol dire che quella mossa migliorerebbe la soluzione
    
    //Si fa una tabu search finche il numero massimo di iterazioni non miglioranti
    //è minore ad un certo parametro (maxNonImprovingIterations) passato
    while(nonImprovingIterations <= maxNonImprovingIterations &&
          ((double)clock() - (double)start)/CLOCKS_PER_SEC + margin < tlim ){
        //La miglior penalita iniziale è la somma di due penalità, usiamo questo
        //Per evitare di mettere un valore troppo basso
        //Mettiamo valore assoluto qualora le due mosse fossero negative
        bestMovePenalty=abs(kempeMovePenaltyWrapper(g, vertex(0, g), 1)) + abs(kempeMovePenaltyWrapper(g, vertex(0, g), 2));
        //Iteriamo sui nodi
        G::Graph::vertex_iterator v, vend;
        
        //Vediamo qual è la miglior mossa fra tutte
        for (boost::tie(v, vend) = vertices(g); v != vend; ++v) {
            iteratedId = get(vertex_index_t(),g,*v);
            color = get(vertex_color_t(),g,*v);
            
            //A volte molte esplorazioni vengono ripetute
            //Devo trovare una maniera di evitare queste ripetizioni
            //Forse conviene segnarmi i colori già esplorati per ogni nodo
            for (iteratedColor = 1; iteratedColor<maxColor;iteratedColor++){
                
                penalty = kempeMovePenaltyWrapper(g, *v, iteratedColor);
                //Verifichiamo che la mossa corrente non sia tabu, e se lo è
                //questa deve soddisfare l'aspiration criterion
                if (iteratedColor == color || (isTabu(iteratedId, iteratedColor) && !aspirationCriterion(accumulatedPenalty, penalty) ))
                    continue;
                    
                if (penalty <= bestMovePenalty){
                    bestMovePenalty = penalty;
                    bestMove = iteratedColor;
                    bestVertex = *v;
                }
                
                
            }
        }
        
        //Fra tutte le mosse possibili effettuo la migliore
         tabuSimpleKempeWrapper(g, bestVertex, bestMove);
         
         //Zero diventa la nuova penalità accumulata a cui si sommano le mosse
         //Per vedere se si giunge all'aspiration criterion
         if(aspirationCriterion(accumulatedPenalty, bestMovePenalty)){
             accumulatedPenalty = 0; //nuova
             nonImprovingIterations = 0; //resettato il numero di iterazioni
             //senza miglioramenti
             setSolution(g, neighborhoodBestSolution);
             neighborhoodBestPenalty = neighborhoodBestSolution.calculatePenalty(g);
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
        return true;
    }
    
    return false;
    
}

void Tabu::tabuIteratedLocalSearch(G::Graph& g, Solution& s,int tollerance,clock_t start,int tlim){
    clock_t current = clock();
    double margin = 3 * ((double)current-(double)start)/CLOCKS_PER_SEC;
    int etamin = 15, etamax = 100;
    int nonImprovingTabus = 0;
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
        perturbate(g, q, eta, s.tmax);
        this->resetTabuList();
        
    }
}
