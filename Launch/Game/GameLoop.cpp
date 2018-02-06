#include "GameLoop.h"
#include "../../Input/GamePadRecorder.h"
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


	GamePadRecorder controllerRecorder;

	while(window->updateWindow() && !window->getKeyboard()->keyDown(KEYBOARD_ESCAPE))
	{

		controllerRecorder.fillInputs();
		 
		controllerRecorder.clearInputs();

		float deltaTime = loopTimer.getTimeSinceLastRetrieval();
		gameSystem.updateNextSystemFrame(deltaTime);

		
		std::cout << "Updated frame " << frameCount << ". Delta time = " << deltaTime << std::endl;
		++frameCount;
	}
}
