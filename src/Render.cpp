#include "Render.h"
#include "SDL_image.h"
#include "Game.h"
#include "World.h"
#include "Actor.h"
#include <vector>
#include <algorithm>
#include <iostream>

Render::Render()
{
}

Render::~Render()
{
}

void Render::Init(Game* gameReference)
{
	game = gameReference;
	renderer = game->GetRenderer();
	WindowHeigh = game->GetWindowHeigh();
	WindowWidth = game->GetWindowWidth();
	GameMapRef = game->GetWorld()->GetGameMap();
	
	//player = (Player) game->GetWorld()->GetPlayer();

	player = dynamic_cast<Player*>(game->GetWorld()->GetPlayer());
	if (!player) {
		printf("Cast failure\n");
	}

	texturesArray[48] = IMG_LoadTexture(renderer, "textures/brick_01.png");
	texturesArray[49] = IMG_LoadTexture(renderer, "textures/brick_02.png");
	texturesArray[50] = IMG_LoadTexture(renderer, "textures/brick_03.png");
	texturesArray[0] = IMG_LoadTexture(renderer, "textures/misc_015.png");
	texturesArray[52] = IMG_LoadTexture(renderer, "textures/misc_09.png");

	distanceToWall = std::vector<float>(WindowWidth, 0);

	pixelsFromMapSquare = game->GetMapProportion();// (WindowWidth / game->GetWorld()->GetMapXSize());
	bRenderMap = false ;
}

void Render::Tick()
{
	SDL_RenderClear(renderer); // Clean the framebuffer
	RenderCeiling();
	RenderFloor();
	RenderWalls();
	RenderActors();
	RenderPlayer();
	RenderDebugMap();
	SDL_RenderPresent(renderer); // Draw the framebuffer
}


void Render::RenderWalls()
{
	for (int i = 0; i < WindowWidth; i++)
	{
		// Create a "ray" from player cordinates to the new x and y, with c distance and certain angle
		// The loop iterate the length of the ray looking for a hitpoint on a wall

		float angle = player->getAngle() - player->GetFov() / 2 + player->GetFov() * i / float(WindowWidth);
		bool bIsTransparent = false;
		float transparentC = 0;
		float transparentX = 0;
		float transparentY = 0;
		unsigned char transparentMapXY = 0;
		for (float c = 0; c < 20; c += .01)
		{
			float x = (float)player->getPositionX() / pixelsFromMapSquare + c * cos(angle); // 32 = WindowWidth/MapLenght
			float y = (float)player->getPositionY() / pixelsFromMapSquare + c * sin(angle);

			unsigned char mapXY = GameMapRef[int(x) + int(y) * 16];

			if(mapXY == '4') 
			{
				if (!bIsTransparent) 
				{
					transparentC = c;
					transparentX = x;
					transparentY = y;
					transparentMapXY = mapXY;
					bIsTransparent = true;
				}
			}
			else if (mapXY != ' ' ) // Check if the ray hits something
			{
				if (bRenderMap) // DEBUG: Draw the map in the screen
				{
					if (mapXY != ' ') 
					{
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
						SDL_RenderDrawLine(renderer, player->getPositionX(), player->getPositionY(), int(x * pixelsFromMapSquare), int(y * pixelsFromMapSquare));
					}
				}
				else
				{
						// Angle correction
						angle -= player->getAngle();
						// angle - player->getAngle-()
						// Size of the column to draw.
						size_t column_height = WindowHeigh / (c * cos(angle));

						SDL_Rect line ;

						getLineToDraw(line, x, y);
						// Draw the textured column

						drawRectangle(1, column_height, i, (WindowHeigh - column_height) / 2, 0xFFFFFF, texturesArray[mapXY], &line);

						if (bIsTransparent)
						{ 
							// Size of the column to draw.
							size_t column_height2 = WindowHeigh / (transparentC * cos(angle));
							SDL_Rect line2;
							getLineToDraw(line2, transparentX, transparentY);
							// Draw the textured column
							drawRectangle(1, column_height2, i, (WindowHeigh - column_height2) / 2, 0xFFFFFF, texturesArray[transparentMapXY], &line2);
						}
						distanceToWall[i] = c* pixelsFromMapSquare;
				}
				break;
			}
		}
	}
}

void Render::RenderPlayer()
{
	if (bRenderMap) { return; }
	if (player->getHasSprite()) 
	{
		int textureSideLenght = 64 * player->GetScaleFactor();
		SDL_Rect dstRect = { 0, 0, textureSideLenght, textureSideLenght };  // Coordinates and size where the texture is going to be drawn
		drawRectangle(textureSideLenght, textureSideLenght, (WindowHeigh/2 - textureSideLenght/2)+player->GetXSpriteOffset(), (WindowWidth/2 - textureSideLenght/2) + player->GetYSpriteOffset(), 0xFFFFFF, player->getSprite(), &dstRect);
	}
}

void Render::RenderActors()
{
	if (bRenderMap) { return; }
	std::vector<Actor*> sortedActors = game->GetWorld()->GetGameActors();
	//std::sort(sortedActors.begin(), sortedActors.end(), CompareTwoSpriteDistanceToPlayer);
	std::sort(sortedActors.begin(), sortedActors.end(), [&](Actor* a,Actor* b) {
		return CompareTwoSpriteDistanceToPlayer(a, b, player);
		});
	//std::sort(sortedActors.begin(), sortedActors.end(), CompareTwoSpriteDistanceToPlayer);

	for (Actor* actor : sortedActors)
	{
		if (actor->getHasSprite())
		{

			// make another ray  
			float expanedfov = player->GetFov() * 1.45;
			float distance = actor->DistanceBetweenActors(player) ;
			float angle2 = player->getAngle();//- player->GetFov() / 2;// +player->GetFov();

			float angleajustadoalcentro = player->getAngle() - player->GetFov() / 2 + player->GetFov() * 256 / float(WindowWidth);

			float x = (float)player->getPositionX() + 20 * cos(angle2);
			float y = (float)player->getPositionY() + 20 * sin(angle2);
			SDL_Rect dstRect = { 0, 0, 64, 64 };  // Coordinates and size where the texture is going to be drawn
			int mainX = WindowHeigh / 2 - 48;
			int mainY = WindowWidth / 2 - 48;

			float vector1X = player->getPositionX() - x;
			float vector1Y = player->getPositionY() - y;
			float vector2X = player->getPositionX() - actor->getPositionX();
			float vector2Y = player->getPositionY() - actor->getPositionY();

			float angulo1 = std::atan2(vector1Y, vector1X);
			float angulo2 = std::atan2(vector2Y, vector2X);
			//SDL_SetRenderDrawColor(renderer, 5, 5, 5, 0);
			//SDL_RenderDrawLine(renderer, player->getPositionX(), player->getPositionY(), x, y);
			//SDL_RenderDrawLine(renderer, player->getPositionX(), player->getPositionY(), actor->getPositionX(), actor->getPositionY());
			float superangle = (angulo2 - angulo1);//* (180.0 / M_PI);
			
			size_t column_height = WindowWidth / (distance * cos(superangle));
			//printf("%f\n", vector1X);
			//printf("%f\n", vector1Y);
			//printf("%f\n", vector2X);
			//printf("%f\n", vector2Y);

			
			float angulo11 = std::atan2(vector1Y, vector1X);
			float angulo22 = std::atan2(vector2Y, vector2X);

			float angleDifference = std::abs(angulo2 - angulo1);
			if (angleDifference > M_PI) {
				// If the difference is greater than PI, we consider the complementary angle
				angleDifference = 2 * M_PI - angleDifference;
				
			}
			mainX = WindowHeigh / (distance / pixelsFromMapSquare * cos(angleDifference));
			

			if (angleDifference <= player->GetFov()/2)
			{
				mainX = WindowHeigh / 2 -64;
				//getfov/2= 30

				if (angulo2 < angulo1) 
				{
					mainX -= mainX * (angleDifference / (player->GetFov() / 2));//percentage on the left
				}
				else 
				{
					mainX += mainX * (angleDifference / (player->GetFov() / 2));//percentage on the right
				}

				angle2 -= player->getAngle();

				// Size of the column to draw.
				size_t column_height = WindowWidth / (distance/ pixelsFromMapSquare * cos(angle2));
				float desplazamientoz = 1 + actor->getPositionZ() / (WindowHeigh / 2); //percentage of movement in Z
				size_t yy = (WindowWidth - column_height) / 2;
				if (actor->DistanceBetweenActors(player) < distanceToWall[mainX])
				{
					drawRectangle(column_height, column_height, mainX, yy * desplazamientoz, 0xFFFFFF, actor->getSprite(), &dstRect);
				}
			}
		}
	}
}

void Render::RenderFloor()
{
	drawRectangle(WindowHeigh, WindowWidth / 2, 0, WindowWidth / 2, 0x323232, nullptr, nullptr);// Draw the floor
}

void Render::RenderCeiling()
{
	drawRectangle(WindowWidth, WindowHeigh / 2, 0, 0, 0x808080, nullptr, nullptr);   // Draw the ceiling
}

void Render::RenderDebugMap()
{
	if (bRenderMap) 
	{
		const size_t mapXLenght = game->GetWorld()->GetMapXSize(); 
		const size_t rect_w = WindowWidth / mapXLenght;
		const size_t rect_h = WindowHeigh / mapXLenght;
		for (size_t j = 0; j < mapXLenght; j++) {
			for (size_t i = 0; i < mapXLenght; i++) {
				if (GameMapRef[i + j * mapXLenght] == ' ') continue; // skip empty spaces
				size_t rect_x = i * rect_w;
				size_t rect_y = j * rect_h;
				drawRectangle(pixelsFromMapSquare, pixelsFromMapSquare, rect_x, rect_y, 0x00FFFF, nullptr, nullptr);
			}
		}
		//Draw player position
		drawRectangle(5, 5, player->getPositionX(), player->getPositionY(), 0x00FFFF, nullptr, nullptr);
		//Draw actors positions
		for (Actor* actor : game->GetWorld()->GetGameActors())
		{
			drawRectangle(5, 5, actor->getPositionX(), actor->getPositionY(), 0x00FF00, nullptr, nullptr);
		}
	}
}

bool Render::CompareTwoSpriteDistanceToPlayer(Actor* actor1, Actor* actor2, Player* player)
{
	return actor1->DistanceBetweenActors(player) < actor2->DistanceBetweenActors(player);
}

void Render::drawRectangle(int width, int high, int x_position, int y_position, uint32_t color, SDL_Texture* tex, SDL_Rect* mask)
{
	SDL_Rect rec = { x_position,y_position,width,high };
	if (tex == nullptr)
	{
		uint8_t r = (color >> 0) & 255;
		uint8_t g = (color >> 8) & 255;
		uint8_t b = (color >> 16) & 255;
		uint8_t a = (color >> 24) & 255;
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderFillRect(renderer, &rec);
	}
	else
	{	//Copy the texture into the render frame
		SDL_RenderCopy(renderer, tex, mask, &rec);
	}
}

void Render::getLineToDraw(SDL_Rect& line, float x, float y)
{
	// Decide if the wall is vertical or horizontal, and calculate the correct texture for the column 
	if (std::abs(y - floor(y + .5)) > std::abs(x - floor(x + .5)))
	{
		line = { int((y - floor(y)) * 64),0,1,64 };
	}
	else
	{
		line = { int((x - floor(x)) * 64),0,1,64 };
	}
}