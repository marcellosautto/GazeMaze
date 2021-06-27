#pragma once
#include "pch.h"
#include "SpriteManager.h"
#include "PrepGame.h"
#include "objDet.h"
#include "animation.h"

// A class that stores the functions for drawing the 'play game' screen
class GameInterface
{
private:

	void drawVideo(sf::RenderTarget& renderTarget),
		drawMaze(sf::RenderTarget& renderTarget);

	bool move(COORD newPos),
		checkCol(COORD newPos),
		input();

	bool mul = false;

	objDet* od = NULL;
	prepGame* pG = NULL;
	TcpSocket* socket = NULL;
	Vector2f* p2Position = NULL;
	Vector2f* prevPosition = NULL;
	char* connectionType = NULL;
	std::array<bool, 5> userInput;

public:

	bool keyboard = false;
	GameInterface(int& _menu, objDet* odt, TcpSocket& TempSocket, Vector2f& Tempp2Position, Vector2f& TempprevPosition, char& TempconnectionType, bool tmul);
	bool Draw(sf::RenderWindow& _window, float _elapsedTime, bool update);

	struct player {

		COORD pos = { 100,50 };

		Vector2f playerVec = { 100.0f, 50.0f };
		animate* playerAni;
		SpriteManager playerSprite;

		player()
		{
			playerSprite.CreateSprite("playerSprite", "Assets/Player/Player1.png");
			playerSprite.GetSprite("playerSprite").setPosition(pos.X, pos.Y);
		}

	}p1, p2;

};
