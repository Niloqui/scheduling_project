#pragma once


#include <algorithm>
#include <random>
#include <iostream>

int integerPower(int arg, int exp) {
	if (exp < 0)
		return 0;

	int res = 1;
	for (int i = 0; i < exp; i++)
		res *= arg;

	return res;
}




