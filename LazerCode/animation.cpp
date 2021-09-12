#include "pch.h"
#include "animation.h"

animate::animate(const sf::Vector2f& pos, bool isMP)
{
	// Set the texture rectangle of the sprite
	sprite.setTextureRect({ 0,0, 50,50 });
	animation[int(animationindex::walkingdown)] = characteranimation(0, 0, 50, 50, isMP);
	animation[int(animationindex::walkingleft)] = characteranimation(0, 50, 50, 50, isMP);
	animation[int(animationindex::walkingright)] = characteranimation(0, 100, 50, 50, isMP);
	animation[int(animationindex::walkingup)] = characteranimation(0, 150, 50, 50, isMP);
	
	
	// Initialize the player sprite

	sprite = sf::Sprite(texture);
	sprite.setPosition(pos);
	sprite.setScale(1.0f, 1.0f);
	

	this->pos = pos;

	
}


void animate::draw(sf::RenderTarget& rt) const
{
	// draw the sprite to the screen
	rt.draw(sprite);
}



void animate::move_player(const sf::Vector2f& pos)
{
	isMoving = (this->pos == pos) ? false : true;//checks if position has changed

	 if (pos.x > this->pos.x)	//walk right
		curAnimation = animationindex::walkingright;
	else if (pos.x < this->pos.x)//walk left
		curAnimation = animationindex::walkingleft;
	else if (pos.y < this->pos.y)//walk down
		curAnimation = animationindex::walkingup;
	else if (pos.y > this->pos.y)//walk up
		curAnimation = animationindex::walkingdown;
	
		
	
	this->pos=pos;
	

}

void animate::update(const float dt)
{
	// Update the animation frames of the sprite

	
	if (isMoving)//updates only if character is moving 
	{
		animation[int(curAnimation)].updatesprite(dt);
		animation[int(curAnimation)].SpriteApplication(sprite);
		sprite.setPosition(this->pos);
	}
	
	

}