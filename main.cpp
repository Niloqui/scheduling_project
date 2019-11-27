#include <iostream>
#include "rd.hpp"
#include "graphw.hpp"
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

int main(int argc, const char * argv[]) {
    
    if(argc!=2){
        std::cout <<"Wrong number of arguments passed"<< std::endl ;
        return -1;
    }
    
    Reader r = Reader(argv[1]);
    G::Graph c = r.read(); //ritorna la matrice dei conlitti
    
    /*
    pair<G::Edge,bool> e = edge(3,13,c); //arco fra i vertici 3 e 13
     if(pair.second){ //vedere che il valore di second sia true, altrimenti non esiste l'arco!
        int w = get(edge_weight_t(),c,e.first); //come ricavare il peso del arco
        //peso = numero di studenti in comune
     }
     */
    
    cout << "Fine " << endl;
    
}
