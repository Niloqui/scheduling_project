#ifndef TROLLEY_H
#define TROLLEY_H
#include "Solution.hpp"
#include "graphw.hpp"
#include <ctime>
#include "LocalSearch.hpp"
#include <stdio.h>

class trolley
{
    public:
        trolley(G::Graph& g,Solution& s,int ts_max,int tSize,int tStep, double currentPenalty);
        trolley(G::Graph& g,Solution& s,int ts_max,double currentPenalty);
        virtual ~trolley();
        void run();
        double getPenalty();
        void changeSize(int);

    protected:
        void firstKnead();
        bool tmove();

    private:
        G::Graph& g = G::Graph(null);
        Solution& s = Solution(null);
        int ts_max;
        bool tEnd;
        int tIndex;
        int tSize;
        int tStep;
        float currentPenalty;
};

#endif // TROLLEY_H
