#include "game.h"

int main(int argc, char* args[])
{
	Game game;

	if (!game.init(512, 512))
	{
		exit(EXIT_FAILURE);
	}

	while (game.running())
	{
		game.handleEvents();
		game.update();
		game.render();
	}

	game.clean();
	return 0;
}