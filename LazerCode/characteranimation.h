#pragma once
#include "pch.h"

// A helper class for the animate class
// Stores and handles the texture frames from a spritesheet
class characteranimation {
public:
	
	characteranimation();
	characteranimation(int x, int y, int width, int height, bool isMP);
	void SpriteApplication(sf::Sprite& s)const;
	void updatesprite(const float dt);
private:
	static constexpr int nFrames = 3;

	sf::Texture texture;
	sf::IntRect frames[nFrames];
	int iFrame = 0;
	float time = 0.0f;
	void Advance();
};