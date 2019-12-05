#pragma once

#include <algorithm>
#include <random>
#include <iostream>
//#include <stdio.h>

std::vector<int> shuffle_vect(std::vector<int> vect){
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (vect.begin(), vect.end(), std::default_random_engine(seed));
    
    return vect;
}
