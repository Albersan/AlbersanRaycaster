#pragma once
#include <SDL.h>
#include <string>
#include <stdio.h>
#include <vector>

class Render;
class World;
class GameMode;
class InputManager;
class SoundManager;

class Game
{
public:
	Game();
	~Game();

	bool Init(int newWindowWidth, int newWindowHeigh, const float newFramesPerSecond);
	void gameLoop();
	void Tick();
	void clean();
	void Restart();

	SDL_Window* GetWindow() { return window; }
	SDL_Renderer* GetRenderer() { return renderer; }
	World* GetWorld() { return world; }
	GameMode* GetGameMode() { return gameMode; }
	Render* GetRender() { return render; }
	InputManager* GetInputManager() { return inputManager; }
	SoundManager* GetSoundManager() { return soundManager; }
	int GetWindowHeigh() { return windowHeigh; }
	int GetWindowWidth() { return windowWidth; }
	int GetMapProportion() { return 32; }
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	World* world = nullptr;
	GameMode* gameMode = nullptr;
	Render* render = nullptr;
	InputManager* inputManager = nullptr;
	SoundManager* soundManager = nullptr;
	int windowHeigh = 0;
	int windowWidth = 0;
	float framesPerSecond = 0;

	void UpdateWindowName(std::string name);
};