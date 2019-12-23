#ifndef SAfun_hpp
#define SAfun_hpp

#include <stdio.h>
#include <iostream>
#include <math.h>
#include "Solution.hpp"
#include "Kempe.hpp"
#include <ctime>



void initialize(Solution &feasibleSol, G::Graph &graph, const std::string statsFile);

// For a true kempe swap, I should grab 2 non connected exams which
// are sheduled in 2 different timeslots.
// Then I should swap the timeslot of every connected exam which is scheduled in
// one of the 2 choosen timeslots in the 2 groups of connected exams
void swap(Solution &feasibleSol, G::Graph &graph, double currentTemp);

bool acceptanceCriterion(int penaltyMove, double currentTemp);

//e^(-(f'-f)/(T0*f))
//T0 = 0.2->heuristic value of the initial temperature
// The function applies a normalization (by deviding for the current penalty),
// try also min_max normalization
float functionProbability(int penaltyMove, double currentTemp);

float randomProbability();

// "alpha" is alpha ad should belong to [0,1], usually 0.99
// "gamma" is gamma and should belong to [10,20]
// The plateau should be equal to the dimension of the neighbourhood
// TO-DO: FIND DIMENSION OF KEMPE NEIGHBOURHOOD
void saWrapper(Solution &feasibleSol, G::Graph &graph, const double initialTemp, const  double alpha, const double gamma, clock_t start, const clock_t timeLimit, int number_of_students, const std::string filename, const std::string statFile = "");


void statistics(const std::string statsFile, int penalty, double time, int iteration, double temperature, int num_of_students);

#endif /* SAfun_hpp */
