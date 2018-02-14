#pragma once

#include "../Systems/System.h"
#include "../../Utilities/GameTimer.h"
#include "../../Input/Devices/Window.h"




#include <memory>
#include "Scene Management/SceneManager.h"
#include "Rendering/RenderingSystem.h"
#include "../Input/InputManager.h"
#include "GameplaySystem.h"

class GameLoop
{
public:
	GameLoop(System& gameSystem);
	~GameLoop();

	void executeGameLoop();

private:
	SceneManager* scene;

	RenderingSystem* rendering;
	InputManager* inputManager;
	GameplaySystem* gameplay;

	System engine;
	GameTimer loopTimer;
	Window* window;

	Camera* camera;

	float pitch = 0.0f;
	float yaw   = 0.0f;
};

