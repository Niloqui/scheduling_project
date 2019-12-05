#ifndef rd_hpp
#define rd_hpp

#include <stdio.h>
#include <string>
#include <cstring>
#include "graphw.hpp"


class Reader {
    private:
        const char* fname;
        int tmax;
        int exams;
    
    
    public:
        Reader(const char* name); //takes namefile as an input
        G::Graph read();
        int getTmax();
        int getExamN();
    
    };


#endif /* rd_hpp */
