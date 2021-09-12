#pragma once
#include "pch.h"
#include "characteranimation.h"

// Class that handles the spritesheet animation of the main character
class animate
{
public:

	//Constructor
	animate(const sf::Vector2f& pos, bool isMC);

	void draw(sf::RenderTarget& rt) const;
	void move_player(const sf::Vector2f& pos);
	void update(const float dt);

	


private:
	enum class animationindex
	{
		walkingup,
		walkingdown,
		walkingleft,
		walkingright,
		count
	};


	sf::Vector2f pos = { 100.0f,50.0f };
	sf::Texture texture;
	sf::Sprite sprite;
	bool isMoving = true;
	characteranimation animation[int(animationindex::count)];
	animationindex curAnimation = animationindex::walkingdown;
};