#include "game.h"

int main(int argc, char* args[])
{
	Game game;

	// Init the game with size of window
	if (!game.init(512, 512))
	{
		exit(EXIT_FAILURE);
	}

	// Frame counter
	int frames = 0;
	
	while (game.running())
	{
		game.handleEvents();
		game.update();
		game.render();
		printf("%i\n",frames);
		frames++;
	}
	game.clean();
	return 0;
}