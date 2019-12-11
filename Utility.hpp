#pragma once


#include <algorithm>
#include <random>
#include <iostream>

/*int integerPower(int arg, int exp) {
	if (exp < 0)
		return 0;

	int res = 1;
	for (int i = 0; i < exp; i++)
		res *= arg;

	return res;
}*/


int integerPower(int arg, int exp);

void mergeSortFin(int* A, int* B, int n, int a, int b);
void mergeSortRec(int* A, int* B, int n, int i, int j);
void mergeSort(int* A, int n);

void mergeSortFin(int* A1, int* A2, int* B1, int* B2, int n, int a, int b);
void mergeSortRec(int* A1, int* A2, int* B1, int* B2, int n, int i, int j);
void mergeSort(int* A1, int* A2, int n);

void reverseVector(int* vect, int n);




