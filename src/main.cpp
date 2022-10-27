#include <SDL.h>
#include <vector>
#include <stdio.h>

// Rendering window
SDL_Window* window = NULL;
// Surface inside de window
SDL_Surface* screenSurface = NULL;
// Renderer component
SDL_Renderer* renderer = NULL;

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) 
{
	return (a << 24) + (b << 16) + (g << 8) + r;
}

void unpack_color(const uint32_t& color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) 
{
	r = (color >> 0) & 255;
	g = (color >> 8) & 255;
	b = (color >> 16) & 255;
	a = (color >> 24) & 255;
}

void render(std::vector<uint32_t>& framebuffer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	for (int i = 0; i < 512; ++i)
	{
		for (int j = 0; j < 512; ++j)
		{
			uint8_t r, g, b, a;
			unpack_color(framebuffer[i + j * 512], r, g, b, a);
			SDL_SetRenderDrawColor(renderer, r, g, b, a);
			SDL_RenderDrawPoint(renderer, i, j);
		}
	}
	SDL_RenderPresent(renderer);
}

void drawGradient(std::vector<uint32_t>& framebuffer)
{
	for (size_t j = 0; j < 512; j++) { // fill the screen with color gradients
		for (size_t i = 0; i < 512; i++) {
			uint8_t r = 255 * j / float(512); // varies between 0 and 255 as j sweeps the vertical
			uint8_t g = 255 * i / float(512); // varies between 0 and 255 as i sweeps the horizontal
			uint8_t b = 0;
			framebuffer[i + j * 512] = pack_color(r, g, b);
		}
	}
}

void update()
{
	std::vector<uint32_t> framebuffer(512*512, 255); // image initialized to white
	drawGradient(framebuffer);
	render(framebuffer);
	framebuffer.~vector();
}

void closeSDL()
{
	SDL_DestroyRenderer(renderer);
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
		window = SDL_CreateWindow("AlbersanRaycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char* args[])
{
	initSDL();

	if (window != NULL)
	{
		SDL_Event e;
		bool quit = false;
		while (quit == false) { 
			update();
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) quit = true;
			}
		}
	}
	closeSDL();
	return 0;
}