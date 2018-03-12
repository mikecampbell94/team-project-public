#pragma comment(lib, "Interface.lib")
#pragma comment(lib, "Utilities.lib")
#pragma comment(lib, "Input.lib")
#pragma comment(lib, "Graphics.lib")
#pragma comment(lib, "Communication.lib")
#pragma comment(lib, "Gameplay.lib")
#pragma comment(lib, "Resource Management.lib")
#pragma comment(lib, "Audio.lib")
#pragma comment(lib, "Physics.lib")

#include <enet\enet.h>
#include "Systems\System.h"
#include "Launch/Startup.h"
#include "Communication/SendMessageActionBuilder.h"

int main()
{
	if (enet_initialize() != 0)
	{
		std::cout << "ENET failed to initialize!" << std::endl;
	}

	SendMessageActionBuilder::initialiseBuilders();
	ThreadPool threadPool;
	
	Startup startup(&threadPool);
	startup.initialiseRenderingSystem();

	bool loaded = false;

	threadPool.submitJob([&startup = startup, &loaded = loaded]()
	{
		startup.initialiseSubsystems();
		startup.loadMainMenu();

		loaded = true;
	});


	while (!loaded)
	{
		startup.renderLoadingScreen();
	}

	startup.startRenderingSystem();
	startup.setupMeshes();
	startup.startUserInterface();
	startup.startGameLoop();

    return 0;
}

