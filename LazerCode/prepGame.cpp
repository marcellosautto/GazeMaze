#include "pch.h"
#include "prepGame.h"

prepGame::prepGame()
{
	mazeS.resize(1);
	mazeS.at(0).resize(0);

	mazeV.resize(1);
	mazeV.at(0).resize(0);

	if (!font.loadFromFile("Assets/Calibri 400.ttf"))
		errorMessageExit("Text Load Fail");

	if (!backgroundTexture.loadFromFile("Assets/BackGround.png") ||
		!pathTexture.loadFromFile("Assets/MazeArt/Path/Path.png") ||
		!lineTexture.loadFromFile("Assets/MazeArt/Path/Line.png"))
		errorMessageExit("Texture Load Fail");

	backgroundSprite.setTexture(backgroundTexture);
	pathSprite.setTexture(pathTexture);
	lineSprite.setTexture(lineTexture);

	for (int i = 0; i < hedgeSpriteCount; i++) {
		if (!hedgeTextures.at(i).loadFromFile("Assets/MazeArt/Hedges/Hedge" + to_string(i) + ".png"))
			errorMessageExit("Hedge Texture Load Fail");

		hedgeSpriteVector.at(i).setTexture(hedgeTextures.at(i));
	}

	for (int i = 0; i < playerSpriteCount; i++) {
		if (!playerTextures.at(i).loadFromFile("Assets/Player/Player" + to_string(i) + ".png"))
			errorMessageExit("Hedge Texture Load Fail");

		playerSprites.at(i).setTexture(playerTextures.at(i));
	}

	TEMPreadInMaze();
	buildMaze();
}

void prepGame::buildMaze()
{
	for (int y = 0; y < mazeV.size(); y++) {

		mazeS.resize(mazeS.size() + 1);//Dynamically resizes outer vector 

		for (int x = 0; x < mazeV.at(y).size(); x++) {//assigns sprites to inner vector

			if (mazeV[y][x] == '1')//If equal to 1, hedge, find the surroundings to pick the correct sprite
				mazeS.at(y).emplace_back(hedgeSpriteVector[findSurrounding(y, x)]);

			else if (mazeV[y][x] == '0') {//if 0 assign path, or gate, depending on position

				if (x == 0 || x == (mazeV.at(y).size() - 1))
					mazeS.at(y).emplace_back(lineSprite);

				else
					mazeS.at(y).emplace_back(pathSprite);
			}
		}
	}
}

void prepGame::TEMPreadInMaze()
{
	char inC;
	ifstream inF("Assets/Maze.txt");

	while (!inF.eof()) {//while there are still move values of inF, repeat loop

		inC = inF.get();//assign current value to inC

		if (inC == '\n') //if inC is next line, create a new outer vector
			mazeV.resize(mazeV.size() + 1);

		else//If inC is a 0 or 1 place it to the top of the current vector 
			mazeV.at(mazeV.size() - 1).emplace_back(inC);
	}

	inF.close();//close inF
}

int prepGame::findSurrounding(int y, int x)
{
	int checker = 0;

	if (x - 0 > 0 && mazeV[y][x - 1] == '1')//If there is a value to the left  of x,y add 1 to checker
		checker += 1;

	if (y < mazeV.size() - 1 && mazeV[y + 1][x] == '1')//If there is a value above x,y add 10 to checker
		checker += 10;

	if (x < mazeV.at(y).size() - 1 && mazeV[y][x + 1] == '1')//If there is a value to the right of x,y add 100 to checker
		checker += 100;

	if (y - 0 > 0 && mazeV[y - 1][x] == '1')//If there is a value below x,y add 1000 to checker
		checker += 1000;

	if (checker == 0)//If statement picks correct sprite and returns it number depending on number
		return 1;

	else if (checker == 1)
		return 2;

	else if (checker == 1000)
		return 3;

	else if (checker == 100)
		return 4;

	else if (checker == 10)
		return 5;

	else if (checker == 101)
		return 6;

	else if (checker == 1010)
		return 7;

	else if (checker == 11)
		return 8;

	else if (checker == 1001)
		return 9;

	else if (checker == 1100)
		return 10;

	else if (checker == 110)
		return 11;

	else if (checker == 1101)
		return 12;

	else if (checker == 1110)
		return 13;

	else if (checker == 111)
		return 14;

	else if (checker == 1011)
		return 15;

	else if (checker == 1111)
		return 0;
}