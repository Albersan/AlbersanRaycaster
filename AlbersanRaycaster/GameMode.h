#pragma once
class Game;
class GameMode
{
public:
	GameMode();
	~GameMode();
	bool Init(Game* gameReference);

	void Tick();

	bool getExitGame() { return bExitGame; };
	void setExitGame(bool newExitGame) { bExitGame = newExitGame; };

	void FindAndSetExitBlock();

private:
	bool bExitGame = false;
	Game* game;
	int exitBlockX = 0;
	int exitBlockY = 0;
};

