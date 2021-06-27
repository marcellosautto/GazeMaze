#pragma once
#include "pch.h"
#include "SpriteManager.h"

using namespace sf;
using namespace std;

class prepGame {

private:

#define hedgeSpriteCount 16
#define playerSpriteCount 4
#define m 9
#define n 9

	void buildMaze(),
		generateMaze(),
		createMaze(int M, int N),
		outputTxt(),
		inputTxt();

	int findSurrounding(int y, int x),
		getIdx(int x, int y, vector < pair<int, pair<int, int> > > cell_list);

	char* connectionType;
	TcpSocket* socket;

public:

	SpriteManager hedgeSpriteManager,
		playerSprites,
		Assets;

	Sprite backgroundS;
	Texture backgroundT;

	sf::Text videoTxt, rightTxt, leftTxt, upTxt, downTxt, oTxt;
	sf::Font Ponde, Calibri;

	vector<vector<Sprite>> mazeS;
	vector<vector<char>> mazeV;

	prepGame(char& TempconnectionType, TcpSocket& Tempsocket,bool mul);
	//~prepGame();
};