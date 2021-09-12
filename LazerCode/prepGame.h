#pragma once
#include "pch.h"
#include "SpriteManager.h"
#include "network.h"

using namespace sf;
using namespace std;

class prepGame {

private:

	Network* network = NULL;

#define hedgeSpriteCount 16
#define playerSpriteCount 4
#define m 9
#define n 9

	void buildMaze(),//Assigns sprites to maze 
		generateMaze(), //calls functions to create a rendomly generated maze
		createMaze(int M, int N),//randomly generates a maze
		outputTxt(),//outputs maze to a text file
		inputTxt();//reads in maze from a txt file

	int findSurrounding(int y, int x),//finds surroundings of a sprite to pick a sprite for the maze
		getIdx(int x, int y, vector < pair<int, pair<int, int> > > cell_list);//used in random maze generation

public:

	SpriteManager hedgeSpriteManager,//object for sprites
		playerSprites,
		Assets;

	Sprite backgroundS;
	Texture backgroundT;

	sf::Text videoTxt, rightTxt, leftTxt, upTxt, downTxt, oTxt, timeTxt, levelTxt;//sfml text
	sf::Font Ponde, Calibri,Atarian;//fonts for sfml text

	vector<vector<Sprite>> mazeS;//2d vector of sprites for the maze
	vector<vector<char>> mazeV;//2d vector of 1s and 0s to repersent maze

	prepGame(Network &networkT); //constructor
	void setMultiplayer();
};