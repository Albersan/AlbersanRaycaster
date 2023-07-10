#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Game.h"
#include "Player.h"
#include "World.h"
#include "Key.h"
#include "Enemy.h"
#include "Door.h"


World::World()
{
}

World::~World()
{
}

void World::Init(Game* gameReference)
{
    //LoadMapFromMemory("maps/Level_01.map");
    LoadLevel("maps/Level_01.map");
    game = gameReference;

    player = dynamic_cast<Player*>(Spawn(ActorClassType::PLAYER));
    //corregir doble tick del player
   
    Door* door = dynamic_cast<Door*>(Spawn(ActorClassType::DOOR));
    door->Init(game);    
    door->SetMapX(3);
    door->SetMapY(7);
    door->SetInteractionDistance(32);
    door->SetNeededItem(5);

    Enemy* robot = dynamic_cast<Enemy*>(Spawn(ActorClassType::ENEMY));
    robot->setPosition(288, 320);
}

void World::LoadLevel(const char* MapName)
{
    LoadMapFromMemory("maps/Level_01.map");
}

void World::Tick()
{
    if (player->getCanTick())
    {
        player->Tick();
    }
    for (Actor* actor : GameActors) 
    {
        if (actor->getCanTick()) 
        {
            actor->Tick();
        }
    }
}

char* World::GetGameMap()
{
    return GameMap;
}

void World::SpawnActor(Actor* actor)
{
    GameActors.push_back(actor);
}

Actor* World::Spawn(ActorClassType actorClassType)
{
    Actor* actor;
    switch (actorClassType)
    {
    case ActorClassType::DOOR:
        actor = new Door();
        break;
    case ActorClassType::ENEMY:
        actor = new Enemy();
        break;
    case ActorClassType::KEY:
        actor = new Key();
        break;
    case ActorClassType::PLAYER:
        actor = new Player();
        break;

    default:
        actor = new Actor();
        break;
    }
    actor->Init(game);
    GameActors.push_back(actor);
    return actor;
}

void World::DeleteActor(Actor* actorToDelete)
{
    GameActors.erase(std::remove_if(GameActors.begin(), GameActors.end(),
        [actorToDelete](Actor* actor) {
            return actor == actorToDelete;
        }),
        GameActors.end());
}

void World::ModifyGameMapValue(int x, int y, unsigned char newValue)
{
    GameMap[x+y*16] = newValue;
}

void World::LoadMapFromMemory(const char* MapName)
{
	std::ifstream infile(MapName);
	int x, y;
    infile.seekg(0, std::ios::end);
    int length = infile.tellg();
    infile.seekg(0, std::ios::beg);

    int parameters[2];

    // Create a char array to store the contents of the file
    char* CurrentMap = new char[length];
    // Read the contents of the file and store it in the array
    infile.read(CurrentMap, length);

    std::string aux = "";
    int mapStartingIndex;
    for (int i = 0; i < length; i++) 
    {
        if (CurrentMap[i] == '\n') 
        {
            mapStartingIndex = i + 1;
            break;
        }
        else 
        {
            aux+=CurrentMap[i];
        }
    }
    
    int parametersIndex = 0;
    std::string auxnum = "";
    for (int i = 0; i <= aux.size(); i++) 
    {
        if (CurrentMap[i] != ';') 
        {
            auxnum += CurrentMap[i];
        }
        else 
        {
            parameters[parametersIndex] = std::stoi(auxnum);
            parametersIndex++;
            auxnum = "";
        }
    }
    mapXSize = parameters[0];
    mapYSize = parameters[1];

    GameMap = new char[mapXSize* mapYSize];
    int mapIndex = 0;

    for (int i = mapStartingIndex; i < length-17; i++) 
    {
        if (CurrentMap[i] != '\n' && CurrentMap[i] != '\0')
        {
            GameMap[mapIndex] = CurrentMap[i];
            mapIndex++;
        }
    }
    delete[] CurrentMap;
}