#include <iostream>
#include "Kempe.hpp"
#include "Solution.hpp"
#include "graphw.hpp"
#include <boost/graph/adjacency_list.hpp>
#include "Utility.hpp"
#define PREDECESSOR -1

using namespace boost;


void colorGraph(G::Graph& g,Solution& s){
    long int id;
    int color;
        
    //Itero sui vertici del grafo in ordine di nome
    //E si colorano i grafi con la soluzione iniziale
    
    //Inoltre vertices ritorna un iterator, se ci serve un descriptor
    //basta dereferenziare
    
    G::Graph::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(g); v != vend; ++v) {
        id = get(vertex_index_t(),g,*v);
        color = s.sol[id];
        put(vertex_color_t(),g,*v,s.sol[id]);
    }
}

void setSolution(G::Graph& g, Solution& s){
    long int id;
    int color;
        
    
    G::Graph::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(g); v != vend; ++v) {
        id = get(vertex_index_t(),g,*v);
        color = get(vertex_color_t(),g,*v);
        s.sol[id]=color;
    }
    
}



static void simpleKempe(G::Graph& g ,G::Vertex v, int color,unordered_set<long int>& visitedNodes){
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
            simpleKempe(g,*vit,myColor,visitedNodes);
        }
    }
    
    put(vertex_color_t(),g,v,color);
    return;
}

void simpleKempeWrapper(G::Graph& g ,G::Vertex v, int color){
    unordered_set<long int> visitedNodes;
    simpleKempe(g, v, color,visitedNodes);
    return;
}


//Color, il colore che diventerà il vertice 1
//V2 è un vertice adiacente
static int colorDistance(G::Graph& g,int color, G::Vertex v2){
    return abs(color-get(vertex_color_t(),g,v2));
}
//Overload per calcolare la distanza fra due nodi
static int colorDistance(G::Graph& g,G::Vertex v1, G::Vertex v2){
    return abs(get(vertex_color_t(),g,v1)-get(vertex_color_t(),g,v2));
}

//Ritorna la penalità associata a due vertici
//Data la loro distanza,che non è detto sia quella contenuta nei colori
//Poichè potremmo voler testare delle possibili mosse
static int penaltyFunction(int distance, G::Vertex v1, G::Vertex v2, G::Graph& g){
    int penalty = 0;
    std::pair<G::Edge,bool> p = edge(v1,v2,g);
    
    if (distance <= 5 && p.second){
        int commonStudents = get(edge_weight_t(),g,p.first);
        penalty = integerPower(2, 5-distance);
        penalty = penalty * commonStudents;
    }
    
    return penalty;
}


//Non si tiene in considerazione la penalità fra elementi della catena
//In quanto la distanza relativa fra gli esami rimane invariata
//Per cui non c'è contributo alla delta nella penalità
int nodeMovePenalty(G::Graph& g, G::Vertex v, int color){
    int nodePenalty=0;
    int distance = 0;
    int originalDistance = 0;
    int originalColor = get(vertex_color_t(),g,v);
    int originalCost=0;

    int iteratedColor;
    
    //Iterazione sui nodi adiacenti
    G::Graph::adjacency_iterator vit,vend;
    for(boost::tie(vit,vend) = adjacent_vertices(v,g); vit!=vend; ++vit){
        
        //La mossa non ha effetto nel costo relativo ai nodi
        //Appartenenti alla medesima catena Kempe
        iteratedColor = get(vertex_color_t(),g,*vit);
        //Quel PREDECESSOR rappresenta un nodo già visitato e che quindi non deve essere processato
        if(iteratedColor != color && iteratedColor != PREDECESSOR){
            distance = colorDistance(g, color, *vit);
            nodePenalty += penaltyFunction(distance,v,*vit,g);
            
            originalDistance = colorDistance(g, originalColor, *vit);
            originalCost += penaltyFunction(originalDistance, v, *vit, g);
        }
        
    }
    
    //Se Negativo vuol dire che la mossa mi migliora la soluzione
    return nodePenalty-originalCost;
}

static int kempeMovePenalty(G::Graph& g, G::Vertex v, int color,  unordered_set<long int>& visitedNodes){
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
        //Cerco il colore relativo alla potenziale mossa da effettura
        iteratedColor = get(vertex_color_t(),g,*vit);
        iteratedId = get(vertex_index_t(),g,*vit);
        if (iteratedColor == color && visitedNodes.count(iteratedId) == 0 ){
            penalty += kempeMovePenalty(g, *vit, originalColor,visitedNodes);
        }
    }
    
    put(vertex_color_t(),g,v,originalColor);
    
    return penalty;
}

int kempeMovePenaltyWrapper(G::Graph& g, G::Vertex v, int color){
    unordered_set<long int> visitedNodes;
    int answer = kempeMovePenalty(g, v, color,visitedNodes);
    return answer;
}


int nodeCurrentPenalty(G::Graph& g, G::Vertex v){
    int originalDistance = 0;
    int originalColor = get(vertex_color_t(),g,v);
    int originalCost=0;
    int iteratedColor;
    
    //Iterazione sui nodi adiacenti
    G::Graph::adjacency_iterator vit,vend;
    for(boost::tie(vit,vend) = adjacent_vertices(v,g); vit!=vend; ++vit){
        
        iteratedColor = get(vertex_color_t(),g,*vit);
        originalDistance = colorDistance(g, originalColor, *vit);
        originalCost += penaltyFunction(originalDistance, v, *vit, g);
        
    }
    
    //Se Negativo vuol dire che la mossa mi migliora la soluzione
    return originalCost;
}
