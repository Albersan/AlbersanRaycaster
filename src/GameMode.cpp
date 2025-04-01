#include "GameMode.h"
#include "Player.h"
#include "World.h"
#include "Game.h";

GameMode::GameMode()
{
}

GameMode::~GameMode()
{
}

bool GameMode::Init(Game* gameReference)
{
	game = gameReference;
	FindAndSetExitBlock();
	return true;
}

void GameMode::Tick()
{
	if (game->GetWorld()->GetPlayer()->GetLife() <= 0)
	{
		printf("\nYou have died, the level will restart.\n");
		game->Restart();
	}
	int playerx = game->GetWorld()->GetPlayer()->getPositionX()/32;
	int playery = game->GetWorld()->GetPlayer()->getPositionY()/32;
	if (playerx == exitBlockX && playery == exitBlockY) 
	{
		printf("\nYou have won!\n Thanks for playing\n");
		bExitGame = true;
	}
}

void GameMode::FindAndSetExitBlock()
{
	for (int i = 0; i < game->GetWorld()->GetMapXSize(); i++) 
	{
		for (int j = 0; j < game->GetWorld()->GetMapYSize(); j++)
		{
			if (game->GetWorld()->GetGameMap()[i + (j * game->GetWorld()->GetMapYSize())] == 'E')
			{
				exitBlockX = i;
				exitBlockY = j;
				game->GetWorld()->ModifyGameMapValue(i, j, ' ');
			}
		}
	}
}