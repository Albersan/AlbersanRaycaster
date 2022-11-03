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

	bool running() { return bRunning; }

private:

	SDL_Window* window;
	SDL_Renderer* renderer;
	bool bRunning;
	int windowHeigh;
	int windowWidth;

	uint32_t player_x;
	uint32_t player_y;
	float player_a;
	float player_fov = M_PI / 3.;

	int lastSDLTime = 0;


	std::vector<uint32_t> framebuffer;

};