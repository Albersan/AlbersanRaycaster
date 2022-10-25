#include <SDL.h>
#include <stdio.h>

// Rendering window
SDL_Window* window = NULL;

// Surface inside de window
SDL_Surface* screenSurface = NULL;

void update()
{
	//Get window surface
	screenSurface = SDL_GetWindowSurface(window);
	//Fill the surface white
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x0F, 0xFF, 0xFF));
	//Update the surface
	SDL_UpdateWindowSurface(window);
}

void closeSDL()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void initSDL()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("AlbersanRaycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	}
}

int main(int argc, char* args[])
{
	initSDL();

	if (window != NULL)
	{
		SDL_Event e;
		bool quit = false;
		while (quit == false) { //Game loop

			update();

			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) quit = true;
			}
		}
	}

	closeSDL();
	return 0;
}