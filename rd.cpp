#include "rd.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef adjacency_list<listS, vecS,undirectedS,no_property,EdgeWeightProperty> Graph;
typedef Graph::edge_descriptor Edge;

pair<int,int> lineConvert(string line);

Reader::Reader (const char* name):fname(name), tmax(-1), exams(0){}

int Reader::getExamN(){
    
    if(this->exams!=0)
        return this->exams;
    
    int exams = 0;
    
    string fName = std::string(this->fname);
    string exmFile = fName + ".exm";
    string line;
    
    ifstream exmread (exmFile);
    
    if(exmread.is_open()){
        while(getline(exmread,line))
            if(line.length()>1) //dont count the last line, or empty ones
                ++exams;
        this->exams = exams;
    }
    else cout << "Unable to open: " << exmFile << endl;

    return exams;
}


//IF THIS FUNCTIONS RETURNS -1 there is a problem
int Reader::getTmax(){
    
    //If the tmax has been read already, avoid rereading from file
    if(this->tmax!=-1)
        return this->tmax;
    
    //If the tmax value has not been read, then read for the first time
    int tmax;
    string fName = std::string(this->fname);
    string sloFile = fName + ".slo";
    string line;
    
    ifstream sloread (sloFile);
    if(sloread.is_open()){
        getline(sloread,line); //get the number tmax
        tmax = stoi(line);
        sloread.close();
        this->tmax = tmax;
        return tmax;
    }
    else cout << "Unable to open: " << sloFile << endl;
    
    return -1;
}

Graph Reader::read(){
    //
    string fName = std::string(this->fname);
    string stuFile = fName + ".stu";
    string exmFile = fName + ".exm";
    string line;
    string nextline;
    int tmax = getTmax();
    int exams = getExamN();
    int *stuExams ;
    stuExams = new int[tmax]; //list of exams for the current student
    
    Graph conflicts(exams); //Grafo dei conflitti
            
    ifstream sturead (stuFile);
    if (sturead.is_open())
    {
        int student;
        int exam;
        int nextStudent;
        int nextExam;
        int i=0;
        
        getline(sturead,line);
        pair<int,int> stuExam = lineConvert(line);
        student = stuExam.first;
        exam = stuExam.second;
        
    //Reading each line, separating it into the student part
    //and into the exam the student is taking
        
        bool flag = false; //ho finito a processare il corrente studente
      while ( getline (sturead,nextline) || !flag)
      {
          flag = false;
          stuExam = lineConvert(nextline);
          nextStudent = stuExam.first;
          nextExam = stuExam.second;
          
          stuExams[i++]=exam-1;
          
          //Per l'ultima iterazione
          if(sturead.peek()==EOF)
              stuExams[i++]=nextExam-1;
            
          //Whenever we pass onto a new student, start adding the edges
          //pertaining to the current student
          if( nextStudent!=student || sturead.peek()==EOF){
              
              //ADD GRAPH STUFF
              for(int y=0; y<i-1; y++){
                  int exam1 = stuExams[y];
                  for(int x=y+1; x<i; x++){
                      int exam2 = stuExams[x];
                      
                      pair<Edge, bool> ed = edge(exam1,exam2,conflicts);
                      
                      //Add edge if its not present
                      if(ed.second==0)
                          add_edge(exam1,exam2,1,conflicts);
                      else{ //Increment its value otherwise
                          int weight = get(edge_weight_t(), conflicts, ed.first);
                          put(edge_weight_t(), conflicts, ed.first, weight+1);
                      }
                      
                      
                  }
              }
              
              flag = true; // Segna che lo studente corrente è stato processato
              i=0;
          }
          
          exam = nextExam;
          student = nextStudent;
      }
      sturead.close();
    }

    else cout << "Unable to open " << stuFile << endl;
    delete [] stuExams;
    
    return conflicts;
    
}

/*Li viene data una riga del file studente
 ritorna una coppia di cui
 il primo elemento è la matricola dello studente
 il secondo elemento è il codice dell'esame
 **/
pair<int,int> lineConvert(string line){
    int student,exam;
    
    tokenizer<> tok(line);
    tokenizer<>::iterator beg=tok.begin();
    string str = *beg;
    student = stoi(str.erase(0,1));
    str = *(++beg);
    exam = stoi(str);
    
    pair<int,int> stuExm;
    stuExm.first = student;
    stuExm.second = exam;
    
    return stuExm;
}
