#pragma once
#include "pch.h"
#include "SpriteManager.h"
#include "PrepGame.h"
#include "objDet.h"

// A class that stores the functions for drawing the 'play game' screen
class GameInterface
{
private:

	void drawMaze(sf::RenderTarget& renderTarget),
		drawVideo(sf::RenderTarget& renderTarget),
		move(COORD newPos),
		input();

	bool checkCol(COORD newPos);
	prepGame* pG = NULL;
	objDet* od = NULL;

	sf::Text videoTxt;
	sf::Font font;


public:

	GameInterface(int& _menu, objDet* odt);
	void Draw(sf::RenderWindow& _window, float _elapsedTime);
	

	struct player {

		COORD pos = { 100,50 };

		SpriteManager playerS;

		player() {

			playerS.CreateSprite("player", "Assets/Player/Player1.png");
			playerS.GetSprite("player").setPosition(pos.X, pos.Y);
		}

	}p;

};

