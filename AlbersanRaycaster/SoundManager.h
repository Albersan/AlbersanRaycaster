#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

class Game;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	void Init(Game* gameReference);
	void PlaySound(Mix_Chunk* sound);
private:
	Game* game = nullptr;
};