#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "Game.h"

int main(int argc, char* args[])
{
	// Init the game with size of window	
	Game game;
	if (!game.Init(512, 512, 60.0f))
	{
		exit(EXIT_FAILURE);
	}

	// Start the gameloop
	game.gameLoop();
	
	return 0;
}