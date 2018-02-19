
#pragma comment(lib, "Utilities.lib")
#pragma comment(lib, "Input.lib")
#pragma comment(lib, "Graphics.lib")
#pragma comment(lib, "Communication.lib")
#pragma comment(lib, "Gameplay.lib")
#pragma comment(lib, "Resource Management.lib")
#pragma comment(lib, "Audio.lib")

#include "Systems\System.h"
#include "Game/GameLoop.h"

#include "Graphics/Rendering/RenderingSystem.h"

#include "Launch/Startup.h"

int main()
{

	//Startup startup;
	//startup.initialiseBaseSubsystems();
	//startup.startGameLoop();

	//ShutdownClass here
	//shutdown.clearAllSubsystems();
	//shutdown.close(); ?


	System engine;
	GameLoop game(engine);
	game.executeGameLoop();

    return 0;
}

