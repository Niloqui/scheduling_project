#include "Tabu.hpp"
#include "Kempe.hpp"
#include <algorithm>
#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


using namespace std;

Tabu::Tabu(int iterations,int tmax):iterations(iterations),tmax(tmax){}
