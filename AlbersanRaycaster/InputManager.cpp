#include "InputManager.h"
#include "Game.h"
#include "World.h"
#include "Render.h"
#include "GameMode.h"


InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}
void InputManager::Init(Game* gameReference) 
{
	game = gameReference;
}

void InputManager::Tick()
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type)
		{
		case SDL_QUIT: game->GetGameMode()->setExitGame(true);
			break;
		case SDL_KEYDOWN:
			for (KeysFunctionsBinding keyBinding: keysFunctionsBindingVector)
			{
				if (keyBinding.inputKey==e.key.keysym.sym) 
				{
					for (const auto& memberFunctionPointer : keyBinding.memberFunctionPointers) {
						memberFunctionPointer(true);
					}
				}
			}
			switch (e.key.keysym.sym)
			{
			case SDLK_m:
				game->GetRender()->setRenderMap(!game->GetRender()->getRenderMap());
				break;
			case SDLK_ESCAPE: game->GetGameMode()->setExitGame(true);
				break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			for (KeysFunctionsBinding keyBinding : keysFunctionsBindingVector)
			{
				if (keyBinding.inputKey == e.key.keysym.sym)
				{
					for (const auto& memberFunctionPointer : keyBinding.memberFunctionPointers) {
						memberFunctionPointer(false);
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

//void InputManager::AddActorInputBinding(SDL_KeyCode inputKey, Actor* actor, std::vector<void (Actor::*)(bool bIsPressed)> vectoroffunctions)
//{
	//KeysFunctionsBinding newKeyFunctionBinding{ inputKey,actor,vectoroffunctions};
	//keysFunctionsBindingVector.push_back(newKeyFunctionBinding);
//}



