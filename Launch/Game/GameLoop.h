#pragma once

#include "../Systems/System.h"
#include "../../Utilities/GameTimer.h"
#include "../../Input/Devices/Window.h"

#include <memory>
#include "Graphics/Rendering/Renderer.h"

class GameLoop
{
public:
	GameLoop(System& gameSystem);
	~GameLoop();

	void executeGameLoop();

private:

	SceneManager* scene;
	Renderer* renderer;


	System gameSystem;
	GameTimer loopTimer;
	Window* window;


	Camera* camera;

	float pitch = 0.0f;
	float yaw   = 0.0f;
};

