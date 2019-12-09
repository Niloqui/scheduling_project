#ifndef PROVA_1234__UTILITY2_H
#define PROVA_1234__UTILITY2_H

#include <algorithm>
#include <random>
#include <iostream>
#include "stdio.h"
#include "string.h"
#include <chrono>


std::vector<int> shuffle_vect(std::vector<int> vect){
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (vect.begin(), vect.end(), std::default_random_engine(seed));
    
    return vect;
}

std::vector<int> shuffle_v(std::vector<int> vec){
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (vec.begin(), vec.end(), std::default_random_engine(seed));
    return vec;
}


void printv(std::vector<int> v, std::string def = "vector index:value"){
    std::cout << def << std::endl;
    int i = 0;
    for(auto elem : v){
        //std::cout << i << " : " << elem << std::endl;
        std::cout << i << "\t";
        i++;
    }
    std::cout << std::endl;

    for(auto elem : v){
        std::cout << elem << "\t";
        //std::cout << i << " | ";
    }
    std::cout << std::endl;

}

void print_i_v(std::vector<int> ind, std::vector<int> v, std::string def = "vector index:value"){
    std::cout << def << std::endl;
    int i = 0;
    for(auto elem : v){
        //std::cout << i << " : " << elem << std::endl;
        std::cout << i << "\t";
        i++;
    }
    std::cout << std::endl;
    for(auto elem : ind){
        std::cout << elem << "\t";
        //std::cout << i << " | ";
    }
    std::cout << std::endl;
    for(auto elem : v){
        std::cout << elem << "\t";
        //std::cout << i << " | ";
    }
    std::cout << std::endl;

}


#endif //PROVA_1234__UTILITY2_H

