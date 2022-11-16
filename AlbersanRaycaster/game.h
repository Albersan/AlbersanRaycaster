#include <SDL.h>
#include <stdio.h>
#include <vector>

class Game
{
public:
	Game();
	~Game();

	bool init(int wHeigh, int wWidth);

	void render();
	void update();
	void handleEvents();
	void clean();

	void drawRectangle(int width, int high, int x_position, int y_position, uint32_t color, SDL_Texture* tex, SDL_Rect* mask);

	bool running() { return bIsRunning; }

	bool bIsMapVisible = false;

private:

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool bIsRunning = false;
	int windowHeigh = 0;
	int windowWidth = 0;

	uint32_t player_x = 0;
	uint32_t player_y = 0;
	float player_a = 0.0;
	float player_fov = M_PI / 3.;

	int lastSDLTime = 0;

	SDL_Texture* brick_01;
};