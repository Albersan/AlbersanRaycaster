#pragma once
#include "Actor.h"
class Key :
    public Actor
{
public:
    Key();
    ~Key();
    void Tick();

    void SetItemID(int newItemID) { itemID = newItemID; }
    int GetItemID() { return itemID; }

private:
    float highCounter = 0;
    int itemID;
};