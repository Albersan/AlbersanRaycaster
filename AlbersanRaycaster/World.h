#pragma once
#include <vector>
#include "Actor.h" 

class Game;

class World
{
public:
	World();
	~World();

	enum class ActorClassType {
		DOOR,
		ENEMY,
		KEY,
		PLAYER
	};

	void Init(Game* gameReference);
	void LoadLevel(const char* MapName);
	void Tick();
	char* GetGameMap();
	std::vector<Actor*> GetGameActors() { return GameActors; }
	Actor* GetPlayer() { return player; }
	
	void SpawnActor(Actor* actor);
	Actor* Spawn(ActorClassType actorClassType);
	void DeleteActor(Actor* actorToDelete);
	void ModifyGameMapValue(int x, int y, unsigned char newValue);
	int GetMapXSize() { return mapXSize; }
	int GetMapYSize() { return mapYSize; }



private:
	void LoadMapFromMemory(const char* MapName);
	int mapXSize;
	int mapYSize;
	char* GameMap;
	Game* game;
	Actor* player;

	std::vector<Actor*> GameActors;
};