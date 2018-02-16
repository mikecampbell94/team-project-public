
#pragma comment(lib, "Utilities.lib")
#pragma comment(lib, "Input.lib")
#pragma comment(lib, "Graphics.lib")
#pragma comment(lib, "Communication.lib")
#pragma comment(lib, "Gameplay.lib")

#pragma comment(lib, "Audio.lib")

#include "Systems\System.h"
#include "Game/GameLoop.h"

#include "Graphics/Rendering/RenderingSystem.h"

int main()
{
	System engine;
	GameLoop game(engine);

	game.executeGameLoop();

	//testing branch

    return 0;
}

