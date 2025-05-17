#ifndef FRAME_CONTROLLER_H
#define FRAME_CONTROLLER_H

#include <chrono>

class FrameController {
private:
	std::chrono::steady_clock::time_point time1, time2;
	float deltaTime;
	std::chrono::steady_clock sclock;
public:
	FrameController();
	float getDeltaTime();
};

#endif
