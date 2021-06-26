#include "pch.h"
#include "MainMenu.h"

MainMenu::MainMenu(int& _menu) {
	menu = &_menu;

	levelCount = 3;

	// load font
	if (!font.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");

	// initialize the text
	text.setFont(font);
	text.setCharacterSize(100);
	text.setPosition(530, 100);
	text.setFillColor(sf::Color::White);
	text.setString("Laser Eyes");

	alreadySelectedTxt.setFont(font);
	alreadySelectedTxt.setCharacterSize(60);
	alreadySelectedTxt.setFillColor(sf::Color::Green);
	alreadySelectedTxt.setPosition(1300, 0);
	alreadySelectedTxt.setString("SELECTED");

	levelCountT.setFont(font);
	levelCountT.setCharacterSize(100);
	levelCountT.setPosition(1300, 600);
	levelCountT.setFillColor(sf::Color::White);

	// initialize buttons
	buttons.push_back(new UIButton("Assets/Menus/PlayGame.png", 610, 500, 2.5));
	buttons.push_back(new UIButton("Assets/Menus/HighScores.png", 610, 600, 2.5));
	buttons.push_back(new UIButton("Assets/Menus/Exit.png", 610, 1000, 2.5));
	buttons.push_back(new UIButton("Assets/Menus/Mul.png", 610, 700, 2.5));
	buttons.push_back(new UIButton("Assets/Menus/input_motion.png", 610, 800, 2.5));
	buttons.push_back(new UIButton("Assets/Menus/input_keyboard.png", 610, 900, 2.5));

	//// initialize the main menu picture
	sf::Sprite& sprite = mng.CreateSprite("Laser", "Assets/Menus/fire.png");
	sprite.setPosition(640, 200);

	if (!backGroundT.loadFromFile("Assets/Menus/menuBack.png"))
		errorMessageExit("Failed To Load Texture");

	backgroundS.setTexture(backGroundT);


	if (!clickB.loadFromFile("Assets/Audio/click.wav"))
		errorMessageExit("Sound Load Fail");

	clickS.setBuffer(clickB);
}

int MainMenu::Draw(sf::RenderWindow& _window, int in) {

	//checks if keyboard input is read in, runs keyboard function
	if (wait == 0) {
		keyBoardIn(in);
		wait = 22;
	}
	else if (wait > 0)
		wait--;

	_window.draw(backgroundS);

	// check if any buttons are pressed and draw the buttons to screen

	for (short i = 0; i < buttons.size(); i++) {
		if (buttons[i]->IsPressed()) {
			*menu = (int)i + 1;
		}
		if (buttons[0]->IsPressed()) {
			*menu = 5;
			clickS.play();
		}
		else if (buttons[4]->IsPressed()) {
			*menu = 6;
		}
		else if (buttons[5]->IsPressed()) {
			*menu = 7;
		}
		buttons[i]->DrawButton(_window);
	}


	// draw the sprites to screen
	mng.DrawSprites(_window);

	// draw the title to screen
	_window.draw(text);

	if (alreadySelectedTxt.getPosition().y != 0)
		_window.draw(alreadySelectedTxt);

	levelCountT.setString(to_string(levelCount));
	_window.draw(levelCountT);
	return levelCount;
}

void MainMenu::keyBoardIn(int in)//determines keyboard input and runs functions
{
	if (in == 8) //if backspace was pressed delete last number input
		levelCount /= 10;

	else if ((in < 58) && (in > 47)) {//If number is entered
		if (levelCount < 10)
			levelCount = (in - 48) + levelCount * 10;
	}
}

void MainMenu::switchControls()
{

}

MainMenu::~MainMenu() {
	// delete the buttons from memory
	for (UIButton* b : buttons) {
		delete b;
	}
}