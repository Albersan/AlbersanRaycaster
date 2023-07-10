#include "Door.h"
#include "InputManager.h"
#include "Game.h"
#include "World.h"
#include "Player.h"
#include <iostream>
#include <vector>

Door::Door()
{
}

Door::~Door()
{
}
void Door::Init(Game* gameReference)
{
	Actor::Init(gameReference);
	gameReference->GetInputManager()->AddActorInputBinding(this, &Door::openTheDoor, SDLK_p);
}
void Door::Tick()
{// Check if the player has the key
	Actor::Tick();
	if (bIsOpen) 
	{
		game->GetWorld()->ModifyGameMapValue(mapX,mapY,' ');
		game->GetWorld()->DeleteActor(this);
	}
	if (PKeyPressed) 
	{
		float result = DistanceBetweenActors(game->GetWorld()->GetPlayer());
		if (result <= interactionDistance && !bIsOpen)
		{
			Player* player = dynamic_cast<Player*>(game->GetWorld()->GetPlayer());
			if (!player)
			{
				printf("Cast failure\n");
			}
			for (int item : player->getItemlist())
			{
				if (item == neededItem)
				{
					bIsOpen = true;
					printf("Opening the door\n");
				}
			}
		}
	}
}

void Door::openTheDoor(bool bIsPressed)
{
	PKeyPressed = bIsPressed;
}