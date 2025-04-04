#pragma once
#include "Actor.h"
class Door :
    public Actor
{
public:
    Door();
    ~Door();

    void Init(Game* gameReference) override;
    void Tick() override;
    int GetMapX() { return mapX; }
    int GetMapY() { return mapY; }

    void SetMapX(int newMapX) { mapX = newMapX; positionX = mapX * 32.f; }
    void SetMapY(int newMapY) { mapY = newMapY; positionY = mapY * 32.f; }

    void openTheDoor(bool bIsPressed);
    void SetNeededItem(int newItem) { neededItem = newItem; }
    int GetNeededItem() { return neededItem; }

private:
    int mapX;
    int mapY;
    bool bIsOpen = false;
    int neededItem;
    bool PKeyPressed = false;
};

