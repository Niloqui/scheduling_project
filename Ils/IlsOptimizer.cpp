#include "IlsOptimizer.hpp"

void ILS::doLocalSearch(){
    this->localSearch->getShuffledExams();
    this->localSearch->firstImprovement();
    this->currentSol = this->localSearch->getSol();
    this->currentPen = this->currentSol->calculatePenalty(this->graph);
}

Solution *ILS::getCurrentSol(){
    return this->currentSol;
}

int ILS::getCurrenPen(){
    return this->currentPen;
}


void ILS::doPerturbation(){
    Solution *newSol;
    int size = this->currentSol->n;
    int vect[size];
    int newPen = -1;
    
    std::vector<int> sol;
      for(int i = 0; i < this->currentSol->n; i++)
          sol.push_back(currentSol->sol[i]);
    
    while (newPen == -1) {
        // se il vettore è piccolo e si sceglie una perturbazione media o piccola (non grande),
        // si può avere come sottovettore in cui fare lo shuffle una sola casella,
        // quindi la funzion shuffle_v farà lo shuffle di un solo elemento e non ha senso,
        // non c'è shuffle in pratica
        this->perturbator->generate_childs(sol,20,40,60);
        
        std::vector<int> tmp_vect = this->perturbator->getLPerturbated();
        
        for(int i = 0; i < this->currentSol->n; i++)
            vect[i] = tmp_vect[i];
        
        newSol = new  Solution(this->currentSol->n, this->currentSol->tmax,vect);
        newPen = newSol->calculatePenalty(this->graph);
    }
    
    
    
    acceptanceTest(newPen, vect);
    
    delete newSol;
    
}

void ILS::acceptanceTest(int newPenality, int *vect){
    if (newPenality < (this->currentPen + 0.5*this->currentPen)) {
        this->currentSol->setSolution(vect);
    }
}
