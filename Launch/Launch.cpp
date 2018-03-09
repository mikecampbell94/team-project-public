#pragma comment(lib, "Interface.lib")
#pragma comment(lib, "Utilities.lib")
#pragma comment(lib, "Input.lib")
#pragma comment(lib, "Graphics.lib")
#pragma comment(lib, "Communication.lib")
#pragma comment(lib, "Gameplay.lib")
#pragma comment(lib, "Resource Management.lib")
#pragma comment(lib, "Audio.lib")
#pragma comment(lib, "Physics.lib")
//#pragma comment(lib, "IPHLPAPI.lib")

#include <enet\enet.h>



#include "Systems\System.h"
#include "Game/GameLoop.h"

#include "Graphics/Rendering/RenderingSystem.h"

#include "Launch/Startup.h"

//#include <iphlpapi.h>
#include <Utilities/NetworkBase.h>
#include "Communication/SendMessageActionBuilder.h"
//#include "../Tuts_Network_Server/NetworkBase.h"
//#include <ENET/include/enet/enet.h>

int main()
{
	if (enet_initialize() != 0)
	{
		std::cout << "ENET failed to initialize!" << std::endl;
	}

	SendMessageActionBuilder::initialiseBuilders();
	

	Startup startup;
	startup.initialiseSubsystems();
	//startup.loadLevel("TestLevel.txt");
	startup.loadMainMenu();
	startup.startGameLoop();

	//ShutdownClass here
	//shutdown.clearAllSubsystems();
	//shutdown.close(); ?


	//System engine;
	//GameLoop game(engine);
	//game.executeGameLoop();

    return 0;
}

