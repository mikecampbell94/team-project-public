#pragma once

#include "../Systems/System.h"
#include "../../Utilities/GameTimer.h"
#include "../../Input/Devices/Window.h"




#include <memory>
#include "Scene Management/SceneManager.h"
#include "Rendering/RenderingSystem.h"
#include "../Input/InputManager.h"
#include "GameplaySystem.h"
#include "../../Interface/UserInterface.h"

#include "../../Audio/AudioSystem.h"
#include "../Communication/MessageProcessor.h"

class GameLoop
{
public:
	GameLoop(System& gameSystem);
	~GameLoop();

	void executeGameLoop();

private:
	MessageProcessor incomingMessages;
	Database* database;
	SceneManager* scene;

	RenderingSystem* rendering;
	InputManager* inputManager;
	GameplaySystem* gameplay;
	AudioSystem* audio;
	UserInterface* userInterface;

	System engine;
	GameTimer loopTimer;
	Window* window;

	Camera* camera;

	bool quit = false;
	float pitch = 0.0f;
	float yaw   = 0.0f;
};

