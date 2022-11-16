#include "game.h"
#include "utils.h"
#include "SDL_image.h"

//TO DO Map Loader
// 
//  Game map of 16x16 blocks
char map[] =
"0000000000000000"\
"03             0"\
"0   0  00000   0"\
"0     0        0"\
"0     0  0000000"\
"0     0        0"\
"004 30000      0"\
"0 0 0   00000  0"\
"0   0   0      0"\
"0   0   0  00000"\
"0       0      0"\
"0       0      0"\
"0       0      0"\
"0 0000000      0"\
"0              0"\
"0000000000000000"; 

Game::Game(){}

Game::~Game(){}

bool Game::init(int wHeigh, int wWidth)
{
	windowHeigh = wHeigh;
	windowWidth = wWidth;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		window = SDL_CreateWindow("AlbersanRaycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowHeigh, windowWidth, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
	}

	player_a = 1.23; // Camera angle from x positive in radians
	player_x = 70;
	player_y = 70;
	bIsRunning = true;

	// Init SDL_IMG and load textures
	IMG_Init(IMG_INIT_PNG);
	brick_01 = IMG_LoadTexture(renderer, "textures/brick_01.png");

	return true;
}

void Game::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) 
		{
		case SDL_QUIT: bIsRunning = false;
			break;

		// Keyboard Events
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) 
			{
			case SDLK_RIGHT: player_a += 2 * M_PI / 360;
				break;
			case SDLK_LEFT: player_a -= 2 * M_PI / 360;
				break;
			case SDLK_UP: player_y += 1;
				break;
			case SDLK_DOWN: player_y -= 1;
				break;
			case SDLK_d: player_x += 1;
				break;
			case SDLK_a: player_x -= 1;
				break;
			case SDLK_SPACE: bIsMapVisible = !bIsMapVisible;
				break;
			case SDLK_ESCAPE: bIsRunning = false;
				break;
			case SDLK_p: map[4 + 6*16] = '3';
				break;
			case SDLK_o: map[4 + 6*16] = ' ';
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
}

void Game::clean()
{   // Close SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::render()
{
	// Wait for reach stable framerate
	while (lastSDLTime - SDL_GetTicks() < 1000/60) { // 60 fps ~ 16ms for frame
		SDL_Delay(1); 	// TODO: improve this 
	}

	SDL_RenderClear(renderer);

	drawRectangle(512, 256, 0, 0, 0x808080,nullptr,nullptr);   // Draw the ceiling
	drawRectangle(512, 256, 0, 256, 0x323232, nullptr,nullptr);// Draw the floor

	if (bIsMapVisible) {
		//Draw map 
		const size_t rect_w = 512 / 16;
		const size_t rect_h = 512 / 16;
		for (size_t j = 0; j < 16; j++) {
			for (size_t i = 0; i < 16; i++) {
				if (map[i + j * 16] == ' ') continue; // skip empty spaces
				size_t rect_x = i * rect_w;
				size_t rect_y = j * rect_h;
				drawRectangle(32, 32, rect_x, rect_y, 0x00FFFF, nullptr, nullptr);
			}
		}
		//Draw player position
		drawRectangle(5, 5, player_x, player_y, 0x00FFFF, nullptr, nullptr);
	}

	for (int i = 0; i < windowWidth; i++)
	{
		float angle = player_a - player_fov / 2 + player_fov * i / float(windowWidth);

		// Create a "ray" from player cordinates to the new x and y, with c distance and certain angle
		// The loop iterate the length of the ray looking for a hitpoint on a wall
		for (float c = 0; c < 20; c += .01) 
		{   
			float x = (float)player_x / 32 + c * cos(angle); 
			float y = (float)player_y / 32 + c * sin(angle);
			char mapXY = map[int(x) + int(y) * 16];

			if (mapXY != ' ') // Check if the ray hits something
			{
				if (bIsMapVisible) // DEBUG: Draw the map in the screen
				{
					if (mapXY == '0') // Draw a normal wall
					{
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
						SDL_RenderDrawLine(renderer, player_x, player_y, int(x * 32), int(y * 32));
					}
					if(mapXY == '3' || mapXY == '4') // Draw diagonal wall
					{   
						// Find the points for the diagonal wall
						int x_diagonal_superior = mapXY == '4' ? (int(x) * 32) : (int(x + 1) * 32);
						int y_diagonal_superior = int(y) * 32;
						int x_diagonal_inferior = mapXY == '4' ? (int(x + 1) * 32) : (int(x) * 32);
						int y_diagonal_inferior = mapXY == '4' ? (int(y + 1) * 32) : (int(y + 1) * 32);
						
						float xd; // X diagonal hitpoint
						float yd; // Y diagonal hitpoint
						
						// Find the intersection between diagonal and the ray and save in xd yd vars
						lineLineIntersection(x_diagonal_superior,y_diagonal_superior,x_diagonal_inferior,y_diagonal_inferior,player_x,player_y,x*32,y*32, xd,yd);
						
						// Continue the foor loop if the intersection is out of diagonal bounds
						if ((xd > x_diagonal_superior&&yd < y_diagonal_superior) || (xd < y_diagonal_inferior && yd > y_diagonal_inferior)){continue;}

						//Draw the diagonal line
						SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0);
						SDL_RenderDrawLine(renderer, x_diagonal_superior, y_diagonal_superior, x_diagonal_inferior, y_diagonal_inferior);
						
						//Draw the ray in blue 
						SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0);
						SDL_RenderDrawLine(renderer, player_x, player_y, int(xd), int(yd));
					}
				}
				else
				{ 
					if (mapXY == '4' || mapXY == '3')
					{
						// Find the points for the diagonal wall
						int x_diagonal_superior = mapXY == '4' ? (int(x) * 32) : (int(x + 1) * 32);
						int y_diagonal_superior = int(y) * 32;
						int x_diagonal_inferior = mapXY == '4' ? (int(x + 1) * 32) : (int(x)*32);
						int y_diagonal_inferior = mapXY == '4' ? (int(y + 1) * 32) : (int(y + 1)*32);

						float xd; // X diagonal hitpoint
						float yd; // Y diagonal hitpoint

						// Find the intersection between diagonal and the ray and save in xd yd vars
						lineLineIntersection(x_diagonal_superior, y_diagonal_superior, x_diagonal_inferior, y_diagonal_inferior, player_x, player_y, x * 32, y * 32, xd, yd);

						// Continue the foor loop if the intersection is out of diagonal bounds
						if ((xd > x_diagonal_superior && yd < y_diagonal_superior) || (xd < y_diagonal_inferior && yd > y_diagonal_inferior)) { continue; }

						// Angle correction
						angle -= player_a;

						// New Distance from player to diagonal hitpoint
						float newC = sqrt(pow(xd - player_x, 2) + pow(yd - player_y, 2) * 1.0) / 32;

						float diagonalDistance = sqrt(pow(x_diagonal_superior - x_diagonal_inferior, 2) + pow(y_diagonal_superior - y_diagonal_inferior, 2) * 1.0);
						float distanceHitToDownCorner = sqrt(pow(xd - x_diagonal_inferior, 2) + pow(yd - y_diagonal_inferior, 2) * 1.0);

						// Size of the column to draw.
						size_t column_heightd = windowWidth / (newC * cos(angle));

						// Calculate texture for the column
						SDL_Rect line = { int((distanceHitToDownCorner / diagonalDistance) * 64),0,1,64 };

						// Draw the textured line
						drawRectangle(1, column_heightd, i, (windowWidth - column_heightd) / 2, 0xFFFFFF, brick_01, &line);
					}
					else 
					{
						// Angle correction
						angle -= player_a;

						// Size of the column to draw.
						size_t column_height = windowWidth / (c * cos(angle));

						SDL_Rect line;

						// Decide if the wall is vertical or horizontal, and calculate the correct texture for the column 
						if (std::abs(y - floor(y + .5)) > std::abs(x - floor(x + .5)))
						{
							line = { int((y - floor(y)) * 64),0,1,64 };
						}
						else
						{
							line = { int((x - floor(x)) * 64),0,1,64 };
						}

						// Draw the textured column
						drawRectangle(1, column_height, i, (windowWidth - column_height) / 2, 0xFFFFFF, brick_01, &line);
					}
				}
				break;
			}
		}
	}
	// Call render 
	SDL_RenderPresent(renderer);

	// Update time
	lastSDLTime = SDL_GetTicks();
}

// Update the game logic
void Game::update()
{	
	
}

//Draw rectangle textured or filled by color
void Game::drawRectangle(int width, int high, int x_position, int y_position, uint32_t color, SDL_Texture* tex, SDL_Rect* mask)
{
	SDL_Rect rec = { x_position,y_position,width,high };
	if (tex == nullptr)
	{
		uint8_t r, g, b, a;
		unpack_color(color, r, g, b, a);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderFillRect(renderer, &rec);
	}
	else
	{	//Copy the texture into the render frame
		SDL_RenderCopy(renderer, tex, mask, &rec);
	}
}