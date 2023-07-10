#pragma once
#include <SDL.h>
#include <stdio.h>
#include "Actor.h"
#include <vector>
#include <functional>


class Game;
class Actor;
class Player;
	
class InputManager
{
public:
	InputManager();
	~InputManager();
	void Init(Game* gameReference);
	void Tick();

	struct KeysFunctionsBinding {
		SDL_KeyCode inputKey;
		Actor* actor;
		std::vector<std::function<void(bool bIsPressed)>> memberFunctionPointers;
	};

	template <typename T>
	void AddActorInputBinding(T* object, void (T::* memberFunction)(bool bIsPressed), SDL_KeyCode inputKey)
	{
		KeysFunctionsBinding newKeyFunctionBinding{inputKey,object};
		newKeyFunctionBinding.memberFunctionPointers.emplace_back([=](bool bIsPressed) 
			{
			(object->*memberFunction)(bIsPressed);
			});
			
		keysFunctionsBindingVector.push_back(newKeyFunctionBinding);
	}

private:
	Game* game;
	std::vector<KeysFunctionsBinding> keysFunctionsBindingVector;
};