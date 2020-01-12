#ifndef rd_hpp
#define rd_hpp

#include "graphw.hpp"

class Reader {
private:
    const char* fname;
    int tmax;
    int exams;
    int students;
    
public:
    Reader(const char* name); //takes namefile as an input
    G::Graph read();
    int getTmax();
    int getExamN();
    int getStudents();
};
    
#endif /* rd_hpp */
