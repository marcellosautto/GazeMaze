#pragma once
#include "pch.h"
#include "HighScore.h"
#include "SpriteManager.h"
#include "Button.h"
#include "MainMenu.h"
#include "ScoreScreen.h"
#include "GameInterface.h"
#include "MultiplayerScreen.h"


class menuSwitch {

public:
	void SFMLLoader(int argc, const char** argv);

private:
	const int frameRate = 256;
};