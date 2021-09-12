#pragma once
#include "pch.h"
#include "SpriteManager.h"

// A class that creates a UI button out of a sprite that can then be clicked to make things happen
class UIButton
{
protected:
	bool pState = false;
	bool isPressed = false;
	bool wasPressed = false;
	
	SpriteManager mng;
	sf::Color color = sf::Color(100, 100, 100);
	sf::SoundBuffer playClickB, defaultClickB;
	sf::Sound playClickS, defaultClickS;

public:
	UIButton(std::string _filepath, float _x, float _y, float _scale = 1);

	bool IsPressed();
	void SetPressColor(sf::Color _color);
	void SetSprite(std::string _filepath);
	void DrawButton(sf::RenderWindow& _window);
};

