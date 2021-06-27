#pragma once
#include "pch.h"
#include "Button.h"
#include "SpriteManager.h"

// All functions for drawing the main menu

class MainMenu {
private:
	sf::Font font;
	sf::Text text;

	sf::Text levelCountT;
	sf::Texture backGroundT;
	sf::Sprite backgroundS;

	std::vector<UIButton*> buttons;
	SpriteManager mng;

	int* menu = nullptr;
	int wait = 0;

	sf::SoundBuffer clickB;
	sf::Sound clickS;


	int levelCount;

public:
	MainMenu(int& _menu);
	int Draw(sf::RenderWindow& _window, int in);
	void keyBoardIn(int in);
	void switchControls();
	~MainMenu();

	sf::Text alreadySelectedTxt;
};