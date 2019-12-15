#include "Tabu.hpp"
#include "Kempe.hpp"


Tabu::Tabu(int iterations):iterations(iterations){}

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
