#include "TimeController.hpp"

TimeController::TimeController(int64_t t) {
	t0 = std::chrono::steady_clock::now();

	tlim = t * 1000 - 500;
}

bool TimeController::isThereTime() {
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(t - t0);

    return time_span.count() < tlim;
}