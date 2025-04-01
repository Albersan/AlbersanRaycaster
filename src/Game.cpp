#include "game.h"
#include "Render.h"
#include "World.h"
#include "InputManager.h"
#include "GameMode.h"
#include "SDL_image.h"
#include <SDL_mixer.h>
#include "SoundManager.h"
 
Game::Game(){}

Game::~Game(){}

bool Game::Init(int newWindowWidth, int newWindowHeigh, const float newFramesPerSecond)
{
	windowHeigh = newWindowHeigh;
	windowWidth = newWindowWidth;
	framesPerSecond = newFramesPerSecond;
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		// Inicializate SDL_mixer
		Mix_Init(MIX_INIT_MP3);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		window = SDL_CreateWindow("AlbersanRaycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeigh, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL)
		{
			printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
	}

	world = new World();
	gameMode = new GameMode();
	inputManager = new InputManager();
	render = new Render();
	soundManager = new SoundManager();

	soundManager->Init(this);
	inputManager->Init(this);
	world->Init(this);
	gameMode->Init(this);
	render->Init(this);
	return true;
}

void Game::gameLoop()
{
	while (!gameMode->getExitGame())
	{
		Uint64 start = SDL_GetPerformanceCounter();
		Tick();
		
		// Update time
		Uint64 end = SDL_GetPerformanceCounter();

		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		// Cap to 60 FPS
		float desiredFrameTime = 1000.0f/framesPerSecond; 
		SDL_Delay(floor(desiredFrameTime - elapsedMS) > 0 ? floor(desiredFrameTime - elapsedMS) : 0);

		end = SDL_GetPerformanceCounter();
		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
		std::string aux = "AlbersanRaycaster " + std::to_string((1.0f / elapsed));
		UpdateWindowName(aux);
	}
	clean();
}

void Game::clean()
{   // Close SDL and delete vars
	delete world;
	delete gameMode;
	delete inputManager;
	delete render;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Game::Restart() 
{
	//delete world;
	//delete gameMode;
	//delete inputManager;
	//delete render;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Game::Init(windowHeigh, windowWidth, framesPerSecond);
}

// Update the game logic
void Game::Tick()
{	
	inputManager->Tick();
	world->Tick();
	gameMode->Tick();
	render->Tick();
}

void Game::UpdateWindowName(std::string name)
{
	SDL_SetWindowTitle(window, name.c_str());
}