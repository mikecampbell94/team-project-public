#pragma once

#include "Windows.h"

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	float getMillisecondsSinceStart();
	float getTimeSinceLastRetrieval();

private:
	LARGE_INTEGER start;
	LARGE_INTEGER ticksPerSecond;
	float timeSinceLastRetrieval;
};

