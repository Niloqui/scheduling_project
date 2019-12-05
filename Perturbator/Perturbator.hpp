#ifndef Perturbator_hpp
#define Perturbator_hpp
#include <iostream>
//#include "Utility2.hpp"
#include "stdio.h"

#include <algorithm>
#include <random>
#include <iostream>


class Perturbator{
    private:
    std::vector<int> previous;
    std::vector<int> previous_indexes;
    std::vector<int> shuffled;
    std::vector<int> shuffled_indexes;
    int vect_size;
    

    std::vector<int> s_perturbated;
    std::vector<int> m_perturbated;
    std::vector<int> l_perturbated;

    std::vector<int> perturbate(int);


    public:
    Perturbator();
    void generate_childs(std::vector<int>, int, int, int);

};

#endif // Perturbator
