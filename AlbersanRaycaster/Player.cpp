#include "Player.h"
#include "Game.h"
#include "World.h"
#include "Enemy.h"
#include "InputManager.h"
#include <cmath>
#include <SDL_image.h>
#include "SoundManager.h"

void Player::Init(Game* gameReference)
{ // HARDCODED
	positionX = 70;
	positionY = 70;
	angle = 1.23f;
	fov = (M_PI / 3.)/512;
	fov *= gameReference->GetWindowWidth();
	bCanTick = true;
	sprite = nullptr;
	life = 100;
	bCanBeDamaged = true;


	Actor::Init(gameReference);
	game->GetInputManager()->AddActorInputBinding(this,&Player::pressArrowDown,SDLK_DOWN);
	game->GetInputManager()->AddActorInputBinding(this, &Player::pressArrowRight,SDLK_RIGHT);
	game->GetInputManager()->AddActorInputBinding(this, &Player::pressArrowLeft, SDLK_LEFT);
	game->GetInputManager()->AddActorInputBinding(this, &Player::pressArrowUp, SDLK_UP);
	game->GetInputManager()->AddActorInputBinding(this, &Player::pressSpaceBar, SDLK_SPACE);

	createAndSetSprite("textures/gun_01.png");
	shotSprite1 = IMG_LoadTexture(game->GetRenderer(), "textures/gun_01.png");
	shotSprite2 = IMG_LoadTexture(game->GetRenderer(), "textures/gun_02.png");
	shotSound = Mix_LoadWAV("sounds/shot.wav");
}

void Player::Tick() 
{
	Actor::Tick();
	int pixelsFromMapSquare = (game->GetWindowWidth() / game->GetWorld()->GetMapXSize());
	if (bIsArrowRightPressed) {
		angle += 8 * M_PI / 360;
	}
	if (bIsArrowLeftPressed)
	{
		angle -= 8 * M_PI / 360;
	}
	if (bIsArrowUpPressed)
	{
		
		float new_x = positionX + (int)5 * cos(angle);
		float new_y = positionY + (int)5 * sin(angle);
		unsigned char mapXY = game->GetWorld()->GetGameMap()[int(new_x / pixelsFromMapSquare) + int(new_y / pixelsFromMapSquare) * 16];
		unsigned char mapXYVertical = game->GetWorld()->GetGameMap()[int(positionX / pixelsFromMapSquare) + int(new_y / pixelsFromMapSquare) * 16];
		unsigned char mapXYHorizontal = game->GetWorld()->GetGameMap()[int(new_x / pixelsFromMapSquare) + int(positionY / pixelsFromMapSquare) * 16];
		
		float proposed_x = positionX;
		float proposed_y = positionY;

		if (mapXY == ' ')
		{
			proposed_x = new_x;
			proposed_y = new_y;
		}
		else if (mapXYVertical == ' ')
		{
			proposed_y = new_y;
		}
		else if (mapXYHorizontal)
		{
			proposed_x = new_x;
		}

		unsigned char FinalMapXY = game->GetWorld()->GetGameMap()[int(proposed_x / pixelsFromMapSquare) + int(proposed_y / pixelsFromMapSquare) * 16];
		if (FinalMapXY == ' ')
		{
			positionX = proposed_x;
			positionY = proposed_y;
		}
	}

	if (bIsArrowDownPressed)
	{
		float new_x = positionX - (int)(5 * cos(angle));
		float new_y = positionY - (int)(5 * sin(angle));

		unsigned char mapXY = game->GetWorld()->GetGameMap()[int(new_x / pixelsFromMapSquare) + int(new_y / pixelsFromMapSquare) * 16];
		if (mapXY == ' ')
		{
			positionX = new_x;
			positionY = new_y;
		}
	}

	if(bIsShooting && DelayCounter == 0)
	{
		shoot();
		game->GetSoundManager()->PlaySound(shotSound);
		DelayCounter = shotDelay;
	}
	if (DelayCounter > 0) 
	{
		setSprite(shotSprite2);
		DelayCounter--;
	}
	else 
	{
		setSprite(shotSprite1);
	}
}

void Player::pressArrowDown(bool bIsPressed)
{
	bIsArrowDownPressed = bIsPressed;
}

void Player::pressArrowUp(bool bIsPressed)
{
	bIsArrowUpPressed = bIsPressed;
}

void Player::pressArrowLeft(bool bIsPressed)
{
	bIsArrowLeftPressed = bIsPressed;
}

void Player::pressArrowRight(bool bIsPressed)
{
	bIsArrowRightPressed = bIsPressed;
}
void Player::pressSpaceBar(bool bIsPressed)
{
	bIsShooting = bIsPressed;
}

void Player::shoot()
{
	for (Actor* actor : game->GetWorld()->GetGameActors())
	{
		Enemy* enemy = dynamic_cast<Enemy*>(actor);
		if (enemy != nullptr) 
		{
			float direction = std::atan2(enemy->getPositionY() - positionY, enemy->getPositionX() - positionX);

			// Calculate the angle difference between the player's direction and rotation.
			float angleDiff = std::abs(angle - direction);

			// Adjust the angle difference to make sure that it is in the range [0, PI)
			angleDiff = std::fmod(angleDiff + M_PI, 2.0 * M_PI) - M_PI;
			if (angleDiff < 0.0) {
				angleDiff = -angleDiff;
			}

			// Check if the angle difference is within the allowed threshold.
			if (angleDiff <= (15 * M_PI / 180.0)) {
				enemy->ApplyDamage(damage);
			}
		}
	}
}