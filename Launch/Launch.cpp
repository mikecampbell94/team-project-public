#pragma comment(lib, "Utilities.lib")
#pragma comment(lib, "Input.lib")

#include "Systems\System.h"
#include "Game/GameLoop.h"

int main()
{
	System engine;
	GameLoop game(engine);

	game.executeGameLoop();

	//testing branch

    return 0;
}

