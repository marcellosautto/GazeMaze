#pragma once
#include "pch.h"

using namespace sf;

class prepGame {

private:

	#define hedgeSpriteCount 16
	#define playerSpriteCount 4

	Texture backgroundTexture;
	Texture pathTexture;
	Texture lineTexture;
	array<Texture, hedgeSpriteCount> hedgeTextures;
	array<Texture, playerSpriteCount> playerTextures;

	vector<vector<char>> mazeV;

	void buildMaze(),
	TEMPreadInMaze();

	int findSurrounding(int y, int x);

public:

	vector<vector<Sprite>> mazeS;
	Sprite backgroundSprite;
	Sprite pathSprite;
	Sprite lineSprite;
	array<Sprite, playerSpriteCount> playerSprites;
	array<Sprite, hedgeSpriteCount> hedgeSpriteVector;

	Font font;

	prepGame();
};