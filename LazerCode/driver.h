#pragma once
#include "pch.h"
#include "HighScore.h"
#include "PhysicsObj.h"
#include "Physics.h"
#include "SpriteManager.h"
#include "Button.h"
#include "MainMenu.h"
#include "ScoreScreen.h"
#include "About.h"
#include "GameInterface.h"


class menuSwitch {

public:

	void SFMLLoader(int argc, const char** argv);

private:
	const int frameRate = 256;
};