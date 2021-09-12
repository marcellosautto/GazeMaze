#pragma once
#include "pch.h"
#include "SpriteManager.h"
#include "Button.h"
#include "network.h"

class MultiplayerScreen {
private:
	int lastKey;//stores the last used key pressed by player
	bool showIP;//shows/hides ip address

	sf::Texture backGroundT;
	sf::Sprite backgroundS;
	std::vector<UIButton*> buttons;
	sf::Time connectTime;
	sf::Font font; 
	sf::Font font2;
	sf::Text connectedT, failedT, IPT, localIPT,waitingT, waitingT2, blink, levelCountT, startFailT, levelT;
	sf::RectangleShape ipCover;
	string enterIP;
	ostringstream uipSS;
	time_t blinkTime;

	void keyBoardIn(int in);//takes keyboard input

	Network* network = NULL;

public:

	MultiplayerScreen(Network& networkT);//constructor
	void Draw(sf::RenderWindow& window, int& menu, int in);//draws and updates screen
};