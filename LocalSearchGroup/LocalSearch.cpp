#include "LocalSearch.hpp"

#include "Kempe.hpp"
#include <iostream>
#include "graphw.hpp"


using namespace std;

void solveFI(G::Graph& g, Solution& s,int iterations){
    int i = iterations;
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

void steepestDescent(G::Graph& g, Solution& s,int iterations){
    int i = iterations;
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

void perturbate(G::Graph& g, int q,int eta, int tmax){
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
        randomColor = rand() % (tmax) + 1; //Colore al quale cambiare
        randomVertex = vertex(randomNode,g);
        
        if(randomColor == get(vertex_color_t(),g,randomVertex))
            randomColor = ((randomColor + 1) % (tmax)) + 1;
        
        simpleKempeWrapper(g, randomVertex, randomColor);
    }
    
    
    delete [] idPenalty;
    return;
}

void iteratedLocalSearch(G::Graph& g, Solution& s, int iterations,int tollerance){
    //Coloriamo il grafo con la soluzione corrente
    Solution newSol(s.n,s.tmax);
    colorGraph(g, s);
    setSolution(g, newSol);
    int pastPenalty = s.calculatePenalty(g); //Penalità iniziale
    int newPenalty,bestPenalty;
    int q=30,eta=10*tollerance;
    
    bestPenalty = pastPenalty;
    for(int n=0; n<iterations; n++){
        
        steepestDescent(g,newSol, tollerance);
        newPenalty = newSol.calculatePenalty(g);
        
            
        if (newPenalty < bestPenalty){
            setSolution(g, s); //Best solution so far is s
            bestPenalty = newPenalty;
        }
        
        if(pastPenalty == newPenalty){ //Perturbazione
            perturbate(g, q, eta, s.tmax);
            setSolution(g, newSol); //Solution currently being explored newSol
        }

        
        pastPenalty = newPenalty;
    }
    
}
