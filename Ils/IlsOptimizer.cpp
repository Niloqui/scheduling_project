#include "IlsOptimizer.hpp"

void ILS::doLocalSearch(){
    this->localSearch->getShuffledExams();
    this->localSearch->firstImprovement();
    this->currentSol = this->localSearch->getSol();
}
