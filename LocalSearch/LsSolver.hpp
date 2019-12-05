#ifndef LsSolver_hpp
#define LsSolver_hpp

//#include "graphw.hpp"
//#include <stdio.h>
//#include <iostream>
#include "Solution.hpp"
//class Solution;

class LS{
    
    private:
    int maxEvals;
    G::Graph graph;
    Solution *solution;
    std::vector<int> exams;
    void generateExams(int);
    bool generateNeighbors(int);
    
    
    
    public:
    LS(G::Graph graph, int maxEvals=-1, Solution *solution = nullptr){
        if(!solution)
            return;
        this->maxEvals = maxEvals;
        this->solution = solution;
        this->generateExams(this->solution->n);
        this->graph = graph;
       }

    /*virtual Solution* GenerateBestNeighbour(const Solution& fatherSolution, int& evalsCounter, int maxNeighbourEvals=-1);
    Solution Solve(Solution initialSolution);*/

    void getShuffledExams();
    void firstImprovement();
    
    // Destructor
    ~LS() { delete[] solution; }

};

#endif /* LsSolver_hpp */
