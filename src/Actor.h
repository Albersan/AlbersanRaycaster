#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include <string>
class Game;
class Actor;

class Actor
{
public:
	Actor();
	~Actor();
	virtual void Init(Game* gameReference);
	virtual void Tick();

	// Getters
	float getPositionX() { return positionX; }
	float getPositionY() { return positionY; }
	float getPositionZ() { return positionZ; }
	float getAngle() { return angle; }
	SDL_Texture* getSprite() { return sprite; }
	bool getCanTick() { return bCanTick; }
	bool getHasSprite() { return sprite != nullptr; }

	// Seters
	void setPositionX(float newPositionX) { positionX = newPositionX; }
	void setPositionY(float newPositionY) { positionY = newPositionY; }
	void setPositionZ(float newPositionZ) { positionZ = newPositionZ; }
	void setPosition(float newPositionX, float newPositionY) { positionX = newPositionX; positionY = newPositionY; }
	
	void setPositionAndAngle(float newPositionX, float newPositionY, float newPositionZ, float newAngle) 
	{ positionX = newPositionX; positionY = newPositionY; 
	positionZ = newPositionZ; angle = newAngle;
	}
	
	void setAngle(float newAngle) { angle = newAngle; }
	void setCanTick(bool newCanTick) { bCanTick = newCanTick; }
	void setSprite(SDL_Texture* newSprite);
	void createAndSetSprite(const char* path);

	float DistanceBetweenActors(Actor* actor);

	int GetLife() { return life; }
	void SetLife(int newLife) { life = newLife; }
	bool GetCanBeDamaged() { return bCanBeDamaged; }
	void SetCanBeDamaged(bool newCanBedamaged) { bCanBeDamaged=newCanBedamaged; }
	void ApplyDamage(int lifeLoss) { if (bCanBeDamaged) life -= lifeLoss; };

	float GetInteractionDistance() { return interactionDistance; }
	void SetInteractionDistance(float newInteractionDistance) { interactionDistance = newInteractionDistance; }

protected:
	float positionX;
	float positionY;
	float positionZ;
	float angle;
	bool bCanTick;
	SDL_Texture* sprite;
	Game* game;
	int life = 100;
	bool bCanBeDamaged = false;
	float interactionDistance = 32;
};