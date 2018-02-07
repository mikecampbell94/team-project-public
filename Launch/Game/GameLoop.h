#pragma once

#include "../Systems/System.h"
#include "../../Utilities/GameTimer.h"
#include "../../Input/Devices/Window.h"




#include <memory>
#include "Scene Management/SceneManager.h"
#include "Rendering/RenderingSystem.h"

class GameLoop
{
public:
	GameLoop(System& gameSystem);
	~GameLoop();

	void executeGameLoop();

private:
	SceneManager* scene;
	RenderingSystem* rendering;

	System engine;
	GameTimer loopTimer;
	Window* window;

	Camera* camera;

	float pitch = 0.0f;
	float yaw   = 0.0f;
};

