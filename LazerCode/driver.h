#pragma once
#include "pch.h"
#include "SpriteManager.h"
#include "Button.h"
#include "MainMenu.h"
#include "ScoreScreen.h"
#include "GameInterface.h"
#include "MultiplayerScreen.h"
#include "ControlScreen.h"
#include "network.h"

bool wait = false;

class SFMLLoader {
public:
	std::array<int, 5> mainLoop(sf::RenderWindow& window);//main loops runs program
	SFMLLoader(int argc, const char** argv, std::array<int, 5> refreshArray);//constructor
	~SFMLLoader();//deconstructor

private:
	//instances of classes
	Network* network = NULL;
	objDet* obDet = NULL;
	MultiplayerScreen* multiplayerScreen = NULL;
	GameInterface* gameInterface = NULL;
	MainMenu* mainMenu = NULL;
	ScoreScreen* scoreScreen = NULL;
	ControlScreen* controlScreen = NULL;

	const int frameRate = 200;
	float nextFrameTime, elpasedTime;
	int menu, pmenu, STime, CTime, threadWaitPMenu, keyboardIn;
	bool update, VisitScore, textWait, pollingKeyboard, pregameWait, refresh, exit, advance;
	steady_clock::time_point Start;
	double MazeTime;

	sf::SoundBuffer musicB;
	sf::Sound musicS;

	sf::Clock clock;
};

class mainLoader {//loads the game 
private:

	sf::Texture backGroundT;
	sf::Sprite backgroundS;
	SFMLLoader* sL = NULL;
	sf::RenderWindow window; // Opens a window

	std::array<int, 5> refreshArray;// 0 Menu, 1 pMenu, 2 levelCount, 3 keyboard, 4 exit the program

	sf::Font font;

	sf::Text waitT;
	
public:
	void loader(int argc, const char** argv);

	mainLoader();

};