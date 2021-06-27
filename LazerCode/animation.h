#pragma once
#include "pch.h"
#include "characteranimation.h"

// Class that handles the spritesheet animation of the main character
class animate
{
public:

	//Constructor
	animate(const sf::Vector2f& pos);

	void draw(sf::RenderTarget& rt) const;

	void setdriection(const sf::Vector2f& dir);

	void update(float dt);


private:
	enum class animationindex
	{
		walkingup,
		walkingdown,
		walkingleft,
		walkingright,
		count
	};

	float speed = 400.0f;

	sf::Vector2f pos;
	sf::Vector2f vel = { 0.0f,0.0f };
	sf::Texture texture;
	sf::Sprite sprite;

	characteranimation animation[int(animationindex::count)];
	animationindex curAnimation = animationindex::walkingdown;
};