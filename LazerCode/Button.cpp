#include "pch.h"
#include "Button.h"

UIButton::UIButton(std::string _filepath, float _x, float _y, float _scale) {
	// Initialize the sprite
	SetSprite(_filepath);
	sf::Sprite& sprite = mng.GetSprite("button");
	sprite.setPosition(_x, _y);
	sprite.setScale(_scale, _scale);
	
	if (!playClickB.loadFromFile("Assets/Audio/p_click.wav"))
		errorMessageExit("Sound Load Fail");

	if (!defaultClickB.loadFromFile("Assets/Audio/click.wav"))
		errorMessageExit("Sound Load Fail");

	playClickS.setBuffer(playClickB);
	defaultClickS.setBuffer(defaultClickB);
}
bool UIButton::IsPressed() {
	// returns true only when the button is released
	return (wasPressed && !isPressed) ? true : false;
}
void UIButton::SetPressColor(sf::Color _color) {
	// sets the color that the button becomes when a user clicks it
	color = _color;
}
void UIButton::SetSprite(std::string _filepath) {
	// Stores sprite in the manager
	mng.CreateSprite("button", _filepath);
}
void UIButton::DrawButton(sf::RenderWindow& _window) {
	// Update the previous state of the button
	wasPressed = isPressed;

	// Check if the user has clicked on the button sprite
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::getPosition(_window).x > mng.GetSprite("button").getPosition().x && sf::Mouse::getPosition(_window).x < mng.GetSprite("button").getPosition().x + mng.GetSprite("button").getGlobalBounds().width && sf::Mouse::getPosition(_window).y > mng.GetSprite("button").getPosition().y && sf::Mouse::getPosition(_window).y < mng.GetSprite("button").getPosition().y + mng.GetSprite("button").getGlobalBounds().height)
		isPressed = true;
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		isPressed = false;

	// Visually change the color of the button when it is pressed
	if (isPressed) {
		mng.GetSprite("button").setColor(color);
		if (mng.GetSprite("button").getPosition() == sf::Vector2f(380, 400))
			playClickS.play();
		else
			defaultClickS.play();
	}
	else {
		mng.GetSprite("button").setColor(sf::Color::White);
	}

	// Draw the button to screen
	_window.draw(mng.GetSprite("button"));
}