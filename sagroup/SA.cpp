#include "SA.hpp"
#include "LocalSearch.hpp"

void swap(Solution &feasibleSol, G::Graph &graph, double currentTemp){
    int n = feasibleSol.n;
    int index1 = rand() % n;
    int timeslot1 = feasibleSol.sol[index1];
    //int index2 = rand() % n;
    //int timeslot2 = feasibleSol.sol[index2];
    int timeslot2 = rand() % feasibleSol.tmax +1 ;
    
    //srand ((unsigned)time(NULL));
    
    while (timeslot2 == timeslot1) {
        //index2 = rand() % n;
        //timeslot2 = feasibleSol.sol[index2];
        timeslot2 = rand() % feasibleSol.tmax +1;
    }
    
    G::Vertex v1 = vertex(index1, graph);
    
    // Penalty of the move
    int penaltyMove = kempeMovePenaltyWrapper(graph, v1, timeslot2);
        
    if (acceptanceCriterion(penaltyMove, currentTemp)){
        simpleKempeWrapper(graph, v1, timeslot2);
        setSolution(graph, feasibleSol);
    }

}



bool acceptanceCriterion(int penaltyMove, double currentTemp) {
     
    if (penaltyMove <= 0) {
        return true;
    }
    
    else if (functionProbability(penaltyMove, currentTemp) > randomProbability()){
        return true;
    }
    
    else
        return false;
}


float functionProbability(int penaltyMove, double currentTemp){
        return exp( -penaltyMove / currentTemp);
}

float randomProbability(){
    // If initialize() has been called, I don't need srand()
    // becase there's a srand() in initializer
    
    //srand ((unsigned)time(NULL));
    float p = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //std::cout << p <<std::endl;
    return p;
}



void initialize(Solution &feasibleSol, G::Graph &graph, const std::string statsFile){
    srand ((unsigned)time(NULL));
    colorGraph(graph, feasibleSol);
    
    if (statsFile != "") {
        std::ofstream file(statsFile);
        file << "Time" << "," << "Iteration" << "," << "Temperature" << "," << "Penalty" << "\n";
        //file.close();
    }
}


void statistics(const std::string statsFile, int penalty, double time, int iteration, double temperature, int num_of_students){
    if (statsFile != "") {
        std::ofstream file(statsFile, std::ios_base::app);
        file << time << "," << iteration << "," << temperature << "," << (double)penalty/num_of_students << "\n";
        //file.close();
    }
}


void saWrapper(Solution &feasibleSol, G::Graph &graph, const double initialTemp, const double alpha, const double gamma, clock_t start, const clock_t timeLimit, int number_of_students, const std::string filename, const std::string statsFile){
    clock_t current = clock();
    double margin = 2 * ((double)current-(double)start)/CLOCKS_PER_SEC;
    
    int ITERATION_COUNTER = 0;
    double currentTemperature = initialTemp;
    // TO-DO: check the size of the kempe neighbourhood
    const int neighbourhood_size = feasibleSol.tmax;
    const int plateau = gamma * neighbourhood_size/4;
    int bestPen, currentPen;
    
    //clock_t start = clock();
    initialize(feasibleSol, graph, statsFile);
    bestPen = feasibleSol.calculatePenalty(graph);
    currentPen = bestPen;
    //swap(feasibleSol, graph, currentTemperature);
    
    while (( (double)std::clock() - (double)start ) / CLOCKS_PER_SEC + margin < timeLimit) {
        
        if (ITERATION_COUNTER % plateau == 0) {
            // k = number_of_iterations/plateau
            currentTemperature = pow(alpha, ITERATION_COUNTER/plateau) * initialTemp;
        }
        
        swap(feasibleSol, graph, currentTemperature);
        steepestDescent(graph, feasibleSol, 20);
        
        currentPen = feasibleSol.calculatePenalty(graph);
        statistics(statsFile, currentPen, (double)std::clock()/CLOCKS_PER_SEC, ITERATION_COUNTER, currentTemperature, number_of_students);
        
        if (bestPen > currentPen) {
            bestPen = currentPen;
            feasibleSol.printSolution(filename);
        }
        
        ITERATION_COUNTER++;
    }
    
    std::cout << "Number of iterations :" << ITERATION_COUNTER << std::endl;
    std::cout << "Number of max times lots :" << feasibleSol.tmax << std::endl;
    
    std::cout <<"Elapsed time: " <<( std::clock() - start ) / (double) CLOCKS_PER_SEC << std::endl;
    std::cout <<"Current temperature: " << currentTemperature << std::endl;
    
    std::cout << "Final solution: " << (double) currentPen/number_of_students << std::endl;
    
    std::cout << "Best solution: " << (double) bestPen/number_of_students << std::endl;
    
}


