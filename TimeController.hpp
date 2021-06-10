#pragma once

#include <chrono>

class TimeController {
public:
	std::chrono::steady_clock::time_point t0;
	int64_t tlim;
	// tlim è espresso in millisecondi

	TimeController(int64_t t);
	// t deve essere espresso in secondi
	// Il costruttore diminuisce in automatico il valore di tlim di mezzo secondo

	bool isThereTime();
};

