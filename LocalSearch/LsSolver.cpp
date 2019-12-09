#include "LsSolver.hpp"
#include <algorithm>
#include <random>
#include <iostream>
//#include "Utility2.hpp"
#include <math.h>
#include <chrono>

//class Solution;

/*Solution *LS::GenerateBestNeighbour(const Solution &fatherSolution, int &evalsCounter, int maxNeighbourEvals) {
    return nullptr;
}*/

Solution *LS::getSol(){
    return this->solution;
}


void LS::generateExams(int n){
    //int *exams = new int[n];
    //for(int i = 0; i < n; i++)
    //    exams[i] = i;
    for(int e=0; e < n; e++){
        this->exams.push_back(e);
    }
    return;
}

void LS::getShuffledExams(){
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    
    shuffle (this->exams.begin(), this->exams.end(), std::default_random_engine(seed));

    //this->exams = shuffle_vect(this->exams);
    
    for(int exam : this->exams)
        std::cout << exam << std::endl;
}


bool LS::generateNeighbors(int index){
    int timeSlots = this->solution->tmax;
    int n = this->solution->n;
    Solution *newsol = new Solution(n, timeSlots, this->solution->sol);
    int currentPenalty = this->solution->calculatePenalty(this->graph);
    int i = 0;
    int penalty = -1;
    bool flag = false;
    
    while (i != timeSlots - 1) {
        
        if (newsol->sol[index] == 0) {
            std::cout << "WTF WTF WTF" << std::endl;
            std::cout << "The timeslot of exam " << index << " is zero!" << std::endl;
            std::cout << "WTF WTF WTF" << std::endl;
        }
        
        newsol->sol[index] = ((((newsol->sol[index] - 1) + 1) % timeSlots) + 1);

        penalty = newsol->calculatePenalty(this->graph);
        
        if (penalty != -1) {
            if (penalty < currentPenalty) {
                this->solution->sol[index] = newsol->sol[index];
                flag = true;
                break;
            }
        }

        i++;
    }
    
    delete newsol;
    
    return flag;
}


void LS::firstImprovement(){
    int maxNUmberOfNeighbors;
    bool better = false;
    int betterCount = 0;
    
    // Change the 10% of the elements of the array
    maxNUmberOfNeighbors = ceil((this->solution->n) * (float)10/100);
    
    for(int i = 0; i < this->solution->n && betterCount < maxNUmberOfNeighbors; i++){
        better = generateNeighbors(this->exams[i]);
        
        if (better)
            betterCount++;
        
    }
    
}
