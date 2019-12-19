#include "trolley.h"


trolley::trolley(G::Graph& g,Solution& s,int ts_max, double currentPenalty)
{
    this->s=s;
    this->g=g;
    this->ts_max=ts_max;
    this->tSize=int(ts_max/3)+1;
    this->tStep=int(ts_max/9)+1;
    this->tIndex=1;
    this->tEnd=false;
    this->currentPenalty=currentPenalty;
}

trolley::trolley(G::Graph& g,Solution& s,int ts_max,int tSize,int tStep, double currentPenalty)
{
    this->s=s;
    this->g=g;
    this->ts_max=ts_max;
    this->tSize=tSize;
    this->tStep=tStep;
    this->tIndex=1;
    this->tEnd=false;
    this->currentPenalty=currentPenalty;
}
void trolley::run(){
    while(this->tmove()){
        this->firstKnead();
    }
}

void trolley::firstKnead(){ //search the local minimum of the window
    for(i=0;i<ts_max;i++){
        int fc=std::rand() % (tStep+1)-tIndex;//fc=first color
        int sc;
        while(int tsc = std::rand() % (tStep+1)-tIndex==fc){
            sc=tsc;
        }
        if(int tmpPenalty = s.calculatePenalty(g)<=this->currentPenalty){ //TODO function name
            swapcolor(this->g,fc,sc,this->s); //TODO function name
            this->currentPenalty = tmpPenalty;
            break;
        }
    }
}

bool trolley::tmove() {
    if (this->tEnd) {
        return false;
    } else if (this->tIndex + this->tStep >= this->ts_max) {
        tEnd = true;
        this->tIndex = this->ts_max - this->tStep;
    } else {
        this->tIndex = this->tIndex + this->tStep;
    }
    return true;
}

void trolley::changeSize(int tSize){
    this->tSize=tSize;
}

double trolley::getPenalty() {
    return this->currentPenalty;
}
