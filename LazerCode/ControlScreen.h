#pragma once
#include "pch.h"
#include "Button.h"
#include "GameInterface.h"
class ControlScreen {
public:

	ControlScreen();
	void draw(sf::RenderWindow& window, bool& threadWait, int& menu, objDet& objDet), //draw control menu
		showDemo(sf::RenderWindow& window, int& menu); //show demonstration video for current controls

	cv::VideoCapture demo; //loads video containing control demos
	bool resetCal; //used for finding out whether the motion controls were reset in the menu or not

	sf::Text selected_txt; //text to show which controls the user currently has enabled

private:

	sf::Text title_txt, skip_txt;
	sf::Font font_ponde;
	std::vector<UIButton*> buttons;

	//stores frames from demo videos
	sf::Image demoImg;
	sf::Texture demoTexture;
	sf::Sprite demoSprite;

	//sfx for invalid selection
	sf::SoundBuffer invalidB;
	sf::Sound invalidS;

	//captured frame goes in here
	cv::Mat video;
};