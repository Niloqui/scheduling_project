#include <iostream>
#include "rd.hpp"
//#include "graphw.hpp"
//#include "Solution.hpp"
//#include <boost/graph/adjacency_list.hpp>
//#include "IlsOptimizer.hpp"
//#include "LsSolver.hpp"
//#include "Perturbator.hpp"
#include "IlsOptimizer.hpp"

#include <ctime>
//#include <iostream>

//void initialSol(G::Graph g);

int main(int argc, const char * argv[]) {
    // insert code here...
    //std::cout << "Hello, World!\n";
    
    if(argc!=2){
        std::cout <<"Wrong number of arguments passed"<< std::endl ;
        return -1;
    }
    
    Reader r = Reader(argv[1]);
    G::Graph c = r.read();
    
   if (strcmp(argv[1], "test") == 0) {
        Solution *sol = new Solution(r.getExamN(), r.getTmax());
        int* vect = new int[4];
       
        vect[0] = 1; vect[1] = 3; vect[2] = 2; vect[3] = 1;
        sol->setSolution(vect);
        std::cout << "penalita': " + std::to_string(sol->calculatePenalty(c)) + "\n";

        vect[0] = 0; vect[1] = 2; vect[2] = 5; vect[3] = 0;
        sol->setSolution(vect);
        std::cout << "penalita': " + std::to_string(sol->calculatePenalty(c)) + "\n";

        vect[0] = 1; vect[1] = 3; vect[2] = 6; vect[3] = 1;
        sol->setSolution(vect);
        std::cout << "penalita': " + std::to_string(sol->calculatePenalty(c)) + "\n";
       
        vect[0] = 1; vect[1] = 3; vect[2] = 2; vect[3] = 1;
        sol->setSolution(vect);
        std::cout << "penalita': " + std::to_string(sol->calculatePenalty(c)) + "\n";
       
       
       ILS *ils = new ILS(c, -1, sol);
       
       int previousPen = -1;
       int counter = 0;
       
       for (int i = 0; i < 50 && counter < 5; i++){
           ils->doLocalSearch();
           ils->doPerturbation();
           
           std::cout << "iteration: " << i+1 << ", penality: " << ils->getCurrenPen() << std::endl;
           std::cout << std::endl;
           
           
           if (previousPen == ils->getCurrenPen()) {
               counter++;
           }
           
           previousPen = ils->getCurrenPen();
       }
        
        /*LS *ls = new LS(c, -1, sol);
        ls->getShuffledExams();
        ls->firstImprovement();*/

       
      // non funzionano i distruttori
       //delete ls;
       //delete sol;
       //delete[] vect;

        //sol.printSolution(filename);
    }
    
    
    
    if (strcmp(argv[1], "instance02") == 0) {
        Solution *sol = new Solution(r.getExamN(), r.getTmax());
        int* vect = new int[sol->n];
        
        unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
        //shuffle (time_slots.begin(), time_slots.end(), std::default_random_engine(seed));
        
        for (int i = 0; i < sol->n; i++) {
            vect[i] = rand() % sol->tmax + 1;
        }
        
        sol->setSolution(vect);
        std::cout << "penalita': " + std::to_string(sol->calculatePenalty(c)) + "\n";
        
        //sol->printS();
        
        LS *ls = new LS(c, -1, sol);
        ls->getShuffledExams();
        ls->firstImprovement();
    }
    
    
    
    

    /*std::vector<int> vect2;
    
    for (int i = 0; i < 30; i++) {
        vect2.push_back(i);
    }
    
    Perturbator *p = new Perturbator();
    p->generate_childs(vect2, 10, 20, 30);*/
    
    /*int *prova = new int[4];
    
    prova[0] = 1;
    prova[1] = 3;
    prova[2] = 5;
    prova[3] = 5;
    
    Solution *S = new Solution(4, 6, prova);
    
    LS ls = LS(-1, S);
    
    ls.getShuffledExams();*/
    
    //initialSol(c);
    
    /*std::vector<int> viva_la_figa;
      for(int i = 0; i < 10; i++)
          viva_la_figa.push_back(i+10);
      Perturbator *p = new Perturbator();
      p->generate_childs(viva_la_figa,20,40,60);
      return 0;
      clock_t start;
      double duration;
      if(argc!=2){
          std::cout <<"Wrong number of arguments passed"<< std::endl ;
          return -1;
      }

      start = clock();

      //Reader r = Reader(argv[1]);
      //G::Graph c = r.read(); //ritorna la matrice dei conlitti

      duration = (double)clock() - (double)start;
      std::cout << "Tempo impiegato per leggere il file: " + std::to_string( duration / CLOCKS_PER_SEC ) + " s\n";
    */
    
    return 0;
}



/*void initialSol(G::Graph g){
    // time_slot-exams
    std::map<int, std::vector<int>> structure;
    std::vector<int> initial_sol;
    std::pair<G::Edge, bool> e;
    
    
    structure[1].push_back(0);
    initial_sol.push_back(1);
    
    for (int n = 1; n < 4; n++) {
        bool flag = false;
        for (int i = 1; i < 6 && !flag; i++) {
            for (int j = 0; j < structure[i].size(); j++) {
                
                e = edge(structure[i][j], n, g);
                
                if (!e.second) {
                    structure[i].push_back(n);
                    initial_sol.push_back(i);
                    flag = true;
                    break;
                }
            }
        
        }
    }
    
    
    
}*/
