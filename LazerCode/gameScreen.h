#pragma once
#include "pch.h"
#include "prepGame.h"

using namespace sf;

class gameScreen {

public:

	bool gameWindow();

private:
	prepGame pG;

	Event windowEvent;
	void drawMaze(RenderTarget& renderTarget);
};