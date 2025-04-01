#include "Enemy.h"
#include "Game.h"
#include "World.h"
#include "Key.h"
#include "Player.h"
#include <cmath>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Tick()
{
	Actor::Tick();
	if(life<=0)
	{
		bCanTick = false;
		
		Key* key = dynamic_cast<Key*>(game->GetWorld()->Spawn(World::ActorClassType::KEY));
		key->setPositionAndAngle(positionX, positionY, 0, 0);
		key->createAndSetSprite("textures/key_01.png");
		key->SetItemID(5);
		game->GetWorld()->DeleteActor(this);
	}

	float targetX = game->GetWorld()->GetPlayer()->getPositionX();
	float targetY = game->GetWorld()->GetPlayer()->getPositionY();

	float diffX = game->GetWorld()->GetPlayer()->getPositionX() - positionX;
	float diffY = game->GetWorld()->GetPlayer()->getPositionY() - positionY;

	float distance = DistanceBetweenActors(game->GetWorld()->GetPlayer());
	diffX /= distance;
	diffY /= distance;

	float speed = 0.2f; 
	float offsetX = diffX * speed;
	float offsetY = diffY * speed;

	float new_x = positionX + offsetX;
	float new_y = positionY + offsetY;

	unsigned char mapXY = game->GetWorld()->GetGameMap()[int(new_x / 32) + int(new_y / 32) * 16];
	if (mapXY == ' ' && distance >=minimumDistanceToPlayer)
	{
		positionX = new_x;
		positionY = new_y;
	}

	if (distance <= minimumDistanceToShoot && DelayCounter == 0) 
	{
		DelayCounter = shotDelay;
		game->GetWorld()->GetPlayer()->ApplyDamage(damage);
		printf("PLAYER DAMAGED, LIFE = %i\n", game->GetWorld()->GetPlayer()->GetLife());
	}
	if (DelayCounter > 0) 
	{
		DelayCounter--;
	}
}

void Enemy::Init(Game* gameReference)
{
	game = gameReference;
	bCanBeDamaged = true;
	createAndSetSprite("textures/enemy_01.png");
	bCanTick = true;
}