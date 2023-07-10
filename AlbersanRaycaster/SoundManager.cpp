#include "SoundManager.h"
#include "Game.h"


SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::Init(Game* gameReference)
{
	game = gameReference;
}

// Play the sound only one time
void SoundManager::PlaySound(Mix_Chunk* sound)
{
	Mix_PlayChannel(-1, sound, 0);  
}
