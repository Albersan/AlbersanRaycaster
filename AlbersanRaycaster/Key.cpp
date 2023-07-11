#include "Key.h"
#include "world.h"
#include "Game.h"
#include "Player.h"
#include <cmath>

Key::Key()
{
}

Key::~Key()
{
}

void Key::Tick()
{
	Actor::Tick();
	positionZ = 50 * std::sin(highCounter);
	highCounter += 0.025;

	if (DistanceBetweenActors(game->GetWorld()->GetPlayer()) <= interactionDistance) 
	{
		Player* player = dynamic_cast<Player*>(game->GetWorld()->GetPlayer());
		if (player) {
			player->AddItem(itemID);
			game->GetWorld()->DeleteActor(this);
		}
	}
}