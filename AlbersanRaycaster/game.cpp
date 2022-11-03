#include "game.h"

char map[] =
"0000222222220000"\
"1              0"\
"1      11111   0"\
"1     0        0"\
"0     0  1110000"\
"0     3        0"\
"0   10000      0"\
"0   0   11100  0"\
"0   0   0      0"\
"0   0   1  00000"\
"0       1      0"\
"2       1      0"\
"0       0      0"\
"0 0000000      0"\
"0              0"\
"0002222222200000"; // our game map

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

void drawGradient(std::vector<uint32_t>& framebuffer)
{
	for (size_t j = 0; j < 512; j++)
	{ // fill the screen with color gradients
		for (size_t i = 0; i < 512; i++)
		{
			uint8_t r = 255 * j / float(512); // varies between 0 and 255 as j sweeps the vertical
			uint8_t g = 255 * i / float(512); // varies between 0 and 255 as i sweeps the horizontal
			uint8_t b = 0;
			framebuffer[i + j * 512] = pack_color(r, g, b);
		}
	}
}

void drawSolidBackgroundColor(std::vector<uint32_t>& framebuffer, uint32_t color)
{ // fill the screen with color gradients
	for (size_t j = 0; j < 512; j++)
	{
		for (size_t i = 0; i < 512; i++)
		{
			framebuffer[i + j * 512] = color;
		}
	}
}

void drawColorRectangle(std::vector<uint32_t>& framebuffer, int width, int high, int x_position, int y_position, uint32_t color)
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < high; ++j)
		{
			framebuffer[x_position + i + (y_position + j) * 512] = color;
		}
	}
}

Game::Game(){}

Game::~Game(){}

bool Game::init(int wHeigh, int wWidth)
{
	windowHeigh = wHeigh;
	windowWidth = wWidth;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		window = SDL_CreateWindow("AlbersanRaycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowHeigh, windowWidth, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
	}

	framebuffer.resize(512*512);// image initialized to black
	player_a = 1.23;
	player_x = 50;
	player_y = 50;
	bRunning = true;
	return true;
}

void Game::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) bRunning = false;
	}
}

void Game::clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::render()
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

	float c = 0;
	for (int a = 0; c < 20; c += .05)
	{
		float x = player_x / 32 + c * cos(player_a);
		float y = player_y / 32 + c * sin(player_a);
		if (map[int(x) + int(y) * 16] != ' ')
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			SDL_RenderDrawLine(renderer, player_x, player_y, int(x) * 32, int(y) * 32);
			break;
		}
	}
	//SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
	//SDL_RenderDrawLine(renderer, player_x, player_y, int(player_x + c * cos(player_a)), int(player_y + c * sin(player_a)));
	SDL_RenderPresent(renderer);
}

void Game::update()
{
	//std::vector<uint32_t> framebuffer(512*512, 255); // image initialized to white
	drawGradient(framebuffer);

	const size_t rect_w = 512 / 16;
	const size_t rect_h = 512 / 16;
	for (size_t j = 0; j < 16; j++) { // draw the map
		for (size_t i = 0; i < 16; i++) {
			if (map[i + j * 16] == ' ') continue; // skip empty spaces
			size_t rect_x = i * rect_w;
			size_t rect_y = j * rect_h;
			drawColorRectangle(framebuffer, 32, 32, rect_x, rect_y, pack_color(0, 255, 255));
		}
	}

	drawColorRectangle(framebuffer, 5, 5, player_x, player_y, pack_color(255, 255, 255));
}