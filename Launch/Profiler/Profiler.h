#pragma once

#include "../Systems/Subsystem.h"
#include "MemoryWatcher.h"

#include <string>
#include <vector>

class Keyboard;
class GameTimer;
class Database;
class FPSCounter;

class Profiler : public Subsystem
{
public:
	Profiler(Keyboard* keyboard, Database* database, FPSCounter* fpsCounter);
	~Profiler() {}

	void updateSubsystem(const float& deltatime) override;
	void addSubsystemTimer(std::string name, GameTimer* timer);

private:
	void updateProfiling();
	
	void displayChildTimers();
	void saveProfilingInfo(GameTimer* parentTimer, int currentDepth, float parentXOffset);

	int numTimers;
	int numAdded = 0;
	int numFramesUpdated = 0;

	Database* database;
	Keyboard* keyboard;

	MemoryWatcher memoryWatcher;
	FPSCounter* fpsCounter;
	map<string, GameTimer*> timers;
	std::vector<TextMeshMessage> messages;

	int depth = -1;
	bool profilerEnabled = false;

	Vector4 defaultColour = Vector4(1, 1, 1, 1);
	Vector2 defaultSize = Vector2(16, 16);
	float nextLine = 0.0f;
};

