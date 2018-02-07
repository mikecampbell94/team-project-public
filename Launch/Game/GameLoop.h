#pragma once

#include "../Systems/System.h"
#include "../../Utilities/GameTimer.h"
#include "../../Input/Devices/Window.h"




#include <memory>

class GameLoop
{
public:
	GameLoop(System& gameSystem);
	~GameLoop();

	void executeGameLoop();

private:
	System gameSystem;
	GameTimer loopTimer;
	Window* window;
};

