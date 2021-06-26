#include "pch.h"
#include "animation.h"

animate::animate(const sf::Vector2f& pos)
{
	// Set the texture rectangle of the sprite
	sprite.setTextureRect({ 0,0, 50,50 });
	animation[int(animationindex::walkingdown)] = characteranimation(50, 0, 50, 50);
	animation[int(animationindex::walkingleft)] = characteranimation(50, 50, 50, 50);
	animation[int(animationindex::walkingright)] = characteranimation(50, 100, 50, 50);
	animation[int(animationindex::walkingup)] = characteranimation(50, 150, 50, 50);

	// Initialize the player sprite
	sprite = sf::Sprite(texture);
	sprite.setPosition(pos);
	sprite.setScale(1.0f, 1.0f);



}

void animate::draw(sf::RenderTarget& rt) const
{
	// draw the sprite to the screen
	rt.draw(sprite);
}

void animate::setdriection(const sf::Vector2f& dir)
{
	// No longer in use in this project :(
	vel = speed * dir;
	if (dir.x > 0.0f)
		curAnimation = animationindex::walkingright;
	else if (dir.x < 0.0f)
		curAnimation = animationindex::walkingleft;
	else if (dir.y < 0.0f)
		curAnimation = animationindex::walkingup;
	else if (dir.y > 0.0f)
		curAnimation = animationindex::walkingdown;
}

void animate::update(float dt)
{
	// Update the animation frames of the sprite

	pos += vel * dt;
	animation[int(curAnimation)].updatesprite(dt);
	animation[int(curAnimation)].SpriteApplication(sprite);
	//sprite.setPosition(pos);
}