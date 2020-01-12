#ifndef Utility_hpp
#define Utility_hpp

int absoluteDifference(int a, int b);

void mergeSortFin(int* A, int* B, int n, int a, int b);
void mergeSortRec(int* A, int* B, int n, int i, int j);
void mergeSort(int* A, int n);

void mergeSortFin(int* A1, int* A2, int* B1, int* B2, int n, int a, int b);
void mergeSortRec(int* A1, int* A2, int* B1, int* B2, int n, int i, int j);
void mergeSort(int* A1, int* A2, int n);

void reverseVector(int* vect, int n);






#endif
