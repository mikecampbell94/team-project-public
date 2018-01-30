#include "GameTimer.h"

GameTimer::GameTimer()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond);
	QueryPerformanceCounter((LARGE_INTEGER*)&start);

	timeSinceLastRetrieval = getMillisecondsSinceStart();
}

GameTimer::~GameTimer()
{
}

float GameTimer::getMillisecondsSinceStart()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);

	return (float)((t.QuadPart - start.QuadPart) * 1000.0 / ticksPerSecond.QuadPart);
}

float GameTimer::getTimeSinceLastRetrieval()
{
	float startTime = getMillisecondsSinceStart();
	float deltaTime = startTime - timeSinceLastRetrieval;

	timeSinceLastRetrieval = startTime;

	return deltaTime;
}
