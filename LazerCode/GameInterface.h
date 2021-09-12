#pragma once
#include "pch.h"
#include "SpriteManager.h"
#include "PrepGame.h"
#include "objDet.h"
#include "animation.h"
#include "network.h"
#include "characteranimation.h"

// A class that stores the functions for drawing the 'play game' screen
class GameInterface
{
private:

	void drawVideo(sf::RenderTarget& renderTarget), //draws video onto the screen
		drawMaze(sf::RenderTarget& renderTarget), //draw maze on the screen
		drawClock(sf::RenderTarget& renderTarget, steady_clock::time_point t1); //draws clock on the screen

	bool move(COORD newPos), //moves player if it does not colide with a wall
		checkCol(COORD newPos),//check if player will colide with a wall
		input(sf::RenderWindow& _window),//checks/interpretes direction user wants to move the player
		newLevel; //reads user input

	//creates instances of classes 
	objDet* od = NULL; 
	Network* network = NULL;
	prepGame* pG = NULL;

public:
	
	GameInterface(int& _menu, objDet* odt, Network &networkT);//constructor
	std::array<bool, 5> userInput; //user array of possible movements 

	bool Draw(sf::RenderWindow& _window, float _elapsedTime, bool update, steady_clock::time_point t1),//draws player on screen and refreshs frame
		completed;

	struct player {//player object

		COORD pos = { 100,50 }; //player position
		sf::Vector2f p_move = {100.0f, 50.0f};//position of annimation
		animate* playerAni = NULL;//instance of animation
		SpriteManager playerSprite;//player sprite

		player()
		{
			playerSprite.CreateSprite("playerSprite", "Assets/Player/eyeball_spritesheet.png");//creates player sprite
		}
	}p1, p2;
};