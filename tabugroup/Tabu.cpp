#include "Tabu.hpp"
#include "Kempe.hpp"
#include <algorithm>
#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


using namespace std;

Tabu::Tabu(int iterations,int tmax):iterations(iterations),tmax(tmax){}

void Tabu::solveFI(G::Graph& g, Solution& s){
    int i = this->iterations;
    int j=0; //Conta le iterazioni
    int maxColor = s.tmax;
    int color,iteratedColor;
    int move=1,movePenalty=0; //miglior penalità ottenuta,
    int penalty; //penalita della mossa corrente
    
    //Coloriamo il grafo con la soluzione corrente
    colorGraph(g, s);
    
    //Iteriamo sui nodi
    G::Graph::vertex_iterator v, vend;
    

    for (boost::tie(v, vend) = vertices(g); v != vend; ++v) {
        color = get(vertex_color_t(),g,*v);
        for (iteratedColor = 1; iteratedColor<maxColor;iteratedColor++){
            if (iteratedColor == color)
                continue;
                
            penalty = kempeMovePenaltyWrapper(g, *v, iteratedColor);
            if (penalty < 0){
                movePenalty = penalty;
                move = iteratedColor;
                simpleKempeWrapper(g, *v, move);
            }
            j++;
            if(j>=i){
                setSolution(g, s);
                return;
                
            }
        
        }
        

    }

    setSolution(g, s);
    return;
}

void Tabu::steepestDescent(G::Graph& g, Solution& s){
    int i = this->iterations;
    int j=0; //Conta le iterazioni
    int maxColor = s.tmax;
    int color,iteratedColor;
    int bestMove=1,bestMovePenalty=0; //miglior penalità ottenuta,
    int penalty; //penalita della mossa corrente
    
    //Coloriamo il grafo con la soluzione corrente
    colorGraph(g, s);
    
    //Iteriamo sui nodi
    G::Graph::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(g); v != vend; ++v) {
        color = get(vertex_color_t(),g,*v);
        bestMovePenalty=0;
        for (iteratedColor = 1; iteratedColor<maxColor;iteratedColor++){
            if (iteratedColor == color)
                continue;
                
            penalty = kempeMovePenaltyWrapper(g, *v, iteratedColor);
            if (penalty < bestMovePenalty){
                bestMovePenalty = penalty;
                bestMove = iteratedColor;
            }
            
        }
        //Faccio il miglior kempe swap possibile per il corrente nodo
        //Solo se c'è una mossa che mi permette di abassare il costo
        //Altrimenti non faccio nienete
        if(bestMovePenalty < 0)
            simpleKempeWrapper(g, *v, bestMove);
        
        j++;
        
        if(j>=i){
            setSolution(g, s);
            return;
            
        }
    }
    
    setSolution(g, s);
}

static bool compare(const pair<long int, int>&i, const pair<long int, int>&j)
{
    return i.second > j.second;
}

void Tabu::perturbate(G::Graph& g, int q,int eta){
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
        randomNode = rand() % q; //Nodo da cambiare
        randomColor = rand() % (this->tmax) + 1; //Colore al quale cambiare
        randomVertex = vertex(randomNode,g);
        
        if(randomColor == get(vertex_color_t(),g,randomVertex))
            randomColor = ((randomColor + 1) % (this->tmax)) + 1;
        
        simpleKempeWrapper(g, randomVertex, randomColor);
    }
    
    
    delete [] idPenalty;
    return;
}
