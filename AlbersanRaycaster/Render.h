#pragma once
#include <SDL.h>
#include "Actor.h"
#include "Player.h"

class Game;

class Render
{
public:
	Render();
	~Render();
	void Init(Game* gameReference);
	void Tick();

	bool getRenderMap() { return bRenderMap; }
	void setRenderMap(bool newRenderMap) {bRenderMap = newRenderMap;}

	static bool CompareTwoSpriteDistanceToPlayer(Actor* actor1, Actor* actor2, Player* player);
private:
	void RenderWalls();
	void RenderPlayer();
	void RenderActors();
	void RenderFloor();
	void RenderCeiling();
	void RenderDebugMap();

	bool bRenderMap = false;
	void drawRectangle(int width, int high, int x_position, int y_position, uint32_t color, SDL_Texture* tex, SDL_Rect* mask);
	SDL_Renderer* renderer = nullptr;
	int WindowHeigh;
	int WindowWidth;
	Game* game;
	Player* player;
	char* GameMapRef;
	void getLineToDraw(SDL_Rect& line, float x,  float y);

	SDL_Texture* texturesArray[255];
	std::vector<float> distanceToWall;
	int pixelsFromMapSquare;
};