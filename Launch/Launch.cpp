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
//#include "../Tuts_Network_Server/NetworkBase.h"
//#include <ENET/include/enet/enet.h>

int main()
{
	NetworkBase network;

	if (enet_initialize() != 0)
	{
		std::cout << "ENET failed to initialize!" << std::endl;
	}


	ENetPeer* serverConnection;// = network.ConnectPeer(127, 0, 0, 1, 1234);

	if (network.Initialize(0))
	{
		serverConnection = network.ConnectPeer(10, 70, 33, 11, 1234);
	}

	while (true)
	{
		network.ServiceNetwork(0, [&serverConnection = serverConnection](const ENetEvent& evnt)
		{
			switch (evnt.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				if (evnt.peer == serverConnection)
				{
					//Send a 'hello' packet
					char* text_data = "Hellooo!";
					ENetPacket* packet = enet_packet_create(text_data, strlen(text_data) + 1, 0);
					enet_peer_send(serverConnection, 0, packet);
				}

				break;
			}
		});
	}

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

