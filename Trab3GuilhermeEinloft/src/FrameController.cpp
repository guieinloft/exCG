#include <time.h>
#include <stdio.h>

#include "FrameController.h"

FrameController::FrameController() {
	time1 = std::chrono::steady_clock::now();
	time2 = std::chrono::steady_clock::now();
	deltaTime = 16;
}

float FrameController::getDeltaTime() {
	time1 = time2;
	time2 = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() / 1000.0f;
	return deltaTime;
}
