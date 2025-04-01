#pragma once
#include "Actor.h"
class Enemy :
    public Actor
{
public:
    Enemy();
    ~Enemy();
    void Tick() override;
    void Init(Game* gameReference) override;

private:
    float minimumDistanceToPlayer = 20;
    float minimumDistanceToShoot = 50;
    float damage = 20;
    int shotDelay = 25;
    int DelayCounter = 0;
};