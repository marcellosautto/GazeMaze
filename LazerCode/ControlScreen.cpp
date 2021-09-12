#include "pch.h"
#include "ControlScreen.h"

ControlScreen::ControlScreen()
{
	resetCal = false;

	//LOAD SFX
	if (!font_ponde.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");

	if (!invalidB.loadFromFile("Assets/Audio/invalid.wav"))
		errorMessageExit("Sound Load Fail");
	invalidS.setBuffer(invalidB);

	//Set font, character size, color, position and strings for text
	title_txt.setFont(font_ponde);
	title_txt.setCharacterSize(120);
	title_txt.setFillColor(sf::Color::Red);
	title_txt.setPosition(350, 50);
	title_txt.setString("CONTROL MENU");

	selected_txt.setFont(font_ponde);
	selected_txt.setCharacterSize(60);
	selected_txt.setFillColor(sf::Color::Green);
	selected_txt.setString("SELECTED");

	skip_txt.setFont(font_ponde);
	skip_txt.setCharacterSize(96);
	skip_txt.setFillColor(sf::Color::Red);
	skip_txt.setPosition(75, 950);
	skip_txt.setString("PRESS 'ESCAPE' TO SKIP");

	//Store Buttons
	buttons.push_back(new UIButton("Assets/Menus/input_motion.png", 200, 400, 0.5));
	buttons.push_back(new UIButton("Assets/Menus/input_keyboard.png", 1200, 400, 0.5));
	buttons.push_back(new UIButton("Assets/Menus/BackToMenu.png", 400, 800, 0.5));
	buttons.push_back(new UIButton("Assets/Menus/demo.png", 1000, 800, 0.5));
	buttons.push_back(new UIButton("Assets/Menus/reset.png", 700, 400, 0.5));

}

void ControlScreen::draw(sf::RenderWindow& window, bool& threadWait, int& menu, objDet& objDet)
{
	// check if any buttons are pressed and draw the buttons to screen
	window.draw(objDet.pregameBGSprite);
	window.draw(title_txt);

	if (buttons[0]->IsPressed()) { //switch to motion controls
		if (objDet.keyboard != false)
		{
			objDet.keyboard = objDet.cameraSwitch(false);
			if (!objDet.keyboard)
			{
				//move selected text to motion controls
				selected_txt.setPosition(225, 250);
				menu = 8;
			}
		}
		else
			invalidS.play();
	}
	else if (buttons[1]->IsPressed()) { //switch to keyboard controls
		if (objDet.keyboard != true)
		{
			objDet.keyboard = objDet.cameraSwitch(true);
			if (objDet.keyboard) 
			{
				//move selected text to keyboard controls
				selected_txt.setPosition(1225, 250);
				menu = 9;
			}
		}

		else
			invalidS.play();
	}
	//Back to Main Menu Button
	else if (buttons[2]->IsPressed())
		menu = 0;

	//Show Demo Button
	else if (buttons[3]->IsPressed())
		if (objDet.keyboard)
			menu = 9;
		else
			menu = 8;

	//Reset Calibration Button
	else if (buttons[4]->IsPressed())
	{
		resetCal = true;
		buttons[4]->DrawButton(window);
		objDet.resetCalibration();
		menu = 5;
		window.clear();
	}

	//Only draw reset button if calibration is set already
	if (objDet.isCalibrated)
		buttons[4]->DrawButton(window);

	for (short i = 0; i < buttons.size() - 1; i++)
		buttons[i]->DrawButton(window);

	if (selected_txt.getPosition().y != 0)
		window.draw(selected_txt);


}

void ControlScreen::showDemo(sf::RenderWindow& window, int& menu)
{
	demo.read(video);
	if (!video.empty())
	{
		cvtColor(video, video, cv::COLOR_BGR2RGBA);
		demoImg.create(video.cols, video.rows, video.ptr());

		if (!demoTexture.loadFromImage(demoImg))
			errorMessageExit("ERROR: texture failed to load");
		demoSprite.setTexture(demoTexture);
		demoSprite.setPosition(0, 0);
		window.draw(demoSprite);
		window.draw(skip_txt);
	}
	else
		menu = 7;
}