#include "Actor.h"
#include "Game.h"
#include "Render.h"
#include "World.h"
#include <cmath>
#include "SDL_image.h"

Actor::Actor() 
{
}
Actor::~Actor()
{
}

void Actor::Init(Game* gameReference)
{
	game = gameReference;
	bCanTick = true;
	sprite = nullptr;
}
void Actor::Tick()
{
}

void Actor::setSprite(SDL_Texture* newSprite)
{
	sprite = newSprite;
}
void Actor::createAndSetSprite(const char* path) 
{
	SDL_Texture* newTexture = IMG_LoadTexture(game->GetRenderer(), path);
	sprite = newTexture;
}

float Actor::DistanceBetweenActors(Actor* actor)
{ // euclidean distance
	float deltaX = getPositionX() - actor->getPositionX();
	float deltaY = getPositionY() - actor->getPositionY();
	return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}