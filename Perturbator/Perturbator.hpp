#ifndef Perturbator_hpp
#define Perturbator_hpp
#include <iostream>
//#include <Utility2.h>
#include "stdio.h"
#include <vector>
#include "Solution.hpp"

class Perturbator {
    private:
    std::vector<int> previous;
    std::vector<int> previous_indexes;
    std::vector<int> shuffled;
    std::vector<int> shuffled_indexes;
    int vect_size;
    //Solution *sol;
    

    std::vector<int> s_perturbated;
    std::vector<int> m_perturbated;
public:
    const std::vector<int> &getSPerturbated() const;

    const std::vector<int> &getMPerturbated() const;

    const std::vector<int> &getLPerturbated() const;

private:
    std::vector<int> l_perturbated;

    std::vector<int> perturbate(int);
    //std::vector<int> generate_feasible_solution(std::vector<int>,G::Graph);


    public:
    Perturbator();
    void generate_childs(std::vector<int>, int, int, int);


};

#endif // Perturbator
