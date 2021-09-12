#pragma once
#include "pch.h"
#include "Button.h"
#include "SpriteManager.h"
#include "GameInterface.h"
#include "objDet.h"
#include "network.h"

// All functions for drawing the main menu

class MainMenu {
private:
	sf::Font font;

	sf::Text levelCountT, waitingT, laserEyesT, alreadySelectedTxt, levelT;
	sf::Texture backGroundT;
	sf::Sprite backgroundS;

	std::vector<UIButton*> buttons;
	SpriteManager mng;

	int* menu = nullptr;
	int totalLevels;

	sf::SoundBuffer clickB;
	sf::Sound clickS;

	objDet* obDet = NULL;
	GameInterface* gameInterface = NULL;
	Network* network = NULL;

public:
	MainMenu(int& _menu, objDet* obDet, GameInterface* gameInterface);//constructor
	int Draw(sf::RenderWindow& _window, int totalLevels);//draws and updates frame

	void waitScreen(sf::RenderWindow& window, Network& networkT);//waiting screen
	~MainMenu();//deconstructor
};