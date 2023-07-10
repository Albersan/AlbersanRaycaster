#pragma once
#include "Actor.h"
class Key :
    public Actor
{
public:
    Key();
    ~Key();
    void Tick();
    float GetInteractionDistance() { return interactionDistance; }
    void SetInteractionDistance(float newInteractionDistance) { interactionDistance = newInteractionDistance; }
    void SetItemID(int newItemID) { itemID = newItemID; }
    int GetItemID() { return itemID; }
private:
    float highCounter = 0;
    float interactionDistance = 0;
    int itemID;
};

