#include "Utility.hpp"


int integerPower(int arg, int exp) {
	if (exp < 0)
		return 0;

	int res = 1;
	for (int i = 0; i < exp; i++)
		res *= arg;

	return res;
}

int64_t integerPower64(int arg, int exp) {
	if (exp < 0)
		return 0;

	int64_t res = 1;
	for (int i = 0; i < exp; i++)
		res *= arg;

	return res;
}

void mergeSortFin(int* A, int* B, int n, int a, int b) {
	int m = (a + b) / 2, i = a, j = m + 1;

	for (int k = a; k <= b; k++) {
		if (i > m) {
			B[k] = A[j];
			j++;
		}
		else if (j > b) {
			B[k] = A[i];
			i++;
		}
		else if (A[i] <= A[j]) {
			B[k] = A[i];
			i++;
		}
		else {
			B[k] = A[j];
			j++;
		}
	}

	for (int k = a; k <= b; k++) {
		A[k] = B[k];
	}
}

void mergeSortRec(int* A, int* B, int n, int i, int j) {
	if (i >= j)
		return;

	mergeSortRec(A, B, n, i, (i + j) / 2);
	mergeSortRec(A, B, n, 1 + (i + j) / 2, j);
	mergeSortFin(A, B, n, i, j);
}

void mergeSort(int* A, int n) {
	int* B = new int[n];

	mergeSortRec(A, B, n, 0, n - 1);
	delete[] B;
}

void mergeSortFin(int* A1, int* A2, int* B1, int* B2, int n, int a, int b) {
	int m = (a + b) / 2, i = a, j = m + 1;

	for (int k = a; k <= b; k++) {
		if (i > m) {
			B2[k] = A2[j];
			B1[k] = A1[j];
			j++;
		}
		else if (j > b) {
			B2[k] = A2[i];
			B1[k] = A1[i];
			i++;
		}
		else if (A2[i] <= A2[j]) {
			B2[k] = A2[i];
			B1[k] = A1[i];
			i++;
		}
		else {
			B2[k] = A2[j];
			B1[k] = A1[j];
			j++;
		}
	}

	for (int k = a; k <= b; k++) {
		A1[k] = B1[k];
		A2[k] = B2[k];
	}
}

void mergeSortRec(int* A1, int* A2, int* B1, int* B2, int n, int i, int j) {
	if (i >= j)
		return;

	mergeSortRec(A1, A2, B1, B2, n, i, (i + j) / 2);
	mergeSortRec(A1, A2, B1, B2, n, 1 + (i + j) / 2, j);
	mergeSortFin(A1, A2, B1, B2, n, i, j);
}

void mergeSort(int* A1, int* A2, int n) {
	int* B1, * B2;
	B1 = new int[n];
	B2 = new int[n];

	mergeSortRec(A1, A2, B1, B2, n, 0, n - 1);
}


void reverseVector(int* vect, int n) {
	int* temp = new int[n];
	int i;

	for (i = 0; i < n; i++)
		temp[i] = vect[i];

	for (i = 0; i < n; i++)
		vect[i] = temp[n-1-i];

	delete [] temp;
}






