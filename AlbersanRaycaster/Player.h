#pragma once
#include "Actor.h"
#include <vector>
class Game;

class Player :
    public Actor
{
public:
	void pressArrowDown(bool bIsPressed);
	void pressArrowUp(bool bIsPressed);
	void pressArrowLeft(bool bIsPressed);
	void pressArrowRight(bool bIsPressed);

	void Tick() override;
	void Init(Game* gameReference) override;
	float GetFov() { return fov; }
	void SetFov(float newFov) { fov = newFov; }

private:
	bool bIsSpaceBarPressed = true;
	bool bIsArrowRightPressed = false;
	bool bIsArrowLeftPressed = false;
	bool bIsArrowUpPressed = false;
	bool bIsArrowDownPressed = false;

	float fov;

	Mix_Chunk* shotSound;

// Examples from Lección 9
public:
	void pressSpaceBar(bool bIsPressed);
	std::vector<int> itemList;
	void AddItem(int newItem) { itemList.push_back(newItem); }
	void DeleteItem(int item) { itemList.erase(std::remove(itemList.begin(), itemList.end(), item), itemList.end()); }
	std::vector<int> getItemlist() { return itemList; }
	int GetLife() { return life; }
	
	float GetScaleFactor() { return scaleFactor; }

	int GetXSpriteOffset() { return XSpriteOffset; }
	int GetYSpriteOffset() { return YSpriteOffset; }

private:
	SDL_Texture* shotSprite1;
	SDL_Texture* shotSprite2;
	int shotDelay = 10;
	int DelayCounter = 0;
	int XSpriteOffset = 0;
	int YSpriteOffset = 128;
	int damage = 25;
	float scaleFactor = 4;
	bool bIsShooting = false;
	void shoot();
};

