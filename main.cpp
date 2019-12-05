#include <iostream>
#include "rd.hpp"
//#include "graphw.hpp"
//#include "Solution.hpp"
//#include <boost/graph/adjacency_list.hpp>
//#include "IlsOptimizer.hpp"
#include "LsSolver.hpp"
//#include "Perturbator.hpp"

//#include <ctime>
//#include <iostream>

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
        
        LS *ls = new LS(c, -1, sol);
        ls->getShuffledExams();
        ls->firstImprovement();

       
      // non funzionano i distruttori
       //delete ls;
       //delete sol;
       //delete[] vect;
       
       

        //sol.printSolution(filename);
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
    
    
    return 0;
}
