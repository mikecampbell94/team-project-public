#include "GameLoop.h"
#include "../../Input/KeyboardRecorder.h"
#include <iostream>

GameLoop::GameLoop(System& gameSystem)
{
	this->gameSystem = gameSystem;
	window = new Window("Game Window");
}

GameLoop::~GameLoop()
{
	delete window;
}

void GameLoop::executeGameLoop()
{
	int frameCount = 0;

	while(window->updateWindow() && !window->getKeyboard()->keyDown(KEYBOARD_ESCAPE))
	{
		float deltaTime = loopTimer.getTimeSinceLastRetrieval();
		gameSystem.updateNextSystemFrame(deltaTime);

		
		std::cout << "Updated frame " << frameCount << ". Delta time = " << deltaTime << std::endl;
		++frameCount;
	}
}
