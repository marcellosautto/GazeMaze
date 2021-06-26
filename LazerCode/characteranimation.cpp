#include "pch.h"
#include "characteranimation.h"


characteranimation::characteranimation() {

}

characteranimation::characteranimation(int x, int y, int width, int height)
{
	// Load texture from file
	texture.loadFromFile("Assets/Player/sword_man.png");


	// Load the frames of the animation
	for (int i = 0; i < nFrames; i++)
	{
		frames[i] = { x + i * width,y,width,height };
	}
}
void characteranimation::SpriteApplication(sf::Sprite& s)const
{
	// Updates a sprites texture to the current texture rectangle
	s.setTexture(texture);
	s.setTextureRect(frames[iFrame]);
}
void characteranimation::updatesprite(float dt)
{
	// Calls the Advance() function if the correct amount of time has passed (based on the animation speed)
	time += dt;
	if (time >= 70) {
		Advance();
		time = 0;
	}
}
void characteranimation::Advance()
{
	// Advances the current frame
	if (++iFrame >= nFrames)
	{
		iFrame = 0;
	}
}