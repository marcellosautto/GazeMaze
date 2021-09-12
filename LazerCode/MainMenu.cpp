#include "pch.h"
#include "MainMenu.h"

MainMenu::MainMenu(int& _menu, objDet* obDetT, GameInterface* gameInterfaceT) {
	menu = &_menu;
	obDet = obDetT;
	gameInterface = gameInterfaceT;

	totalLevels = 3;

	// load font
	if (!font.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");

	// initialize the text
	laserEyesT.setFont(font);
	laserEyesT.setCharacterSize(124);
	laserEyesT.setPosition(510, 75);
	laserEyesT.setFillColor(sf::Color::White);
	laserEyesT.setString("Gaze Maze");

	waitingT.setFont(font);
	waitingT.setCharacterSize(50);
	waitingT.setPosition(100, 500);

	levelCountT.setFont(font);
	levelCountT.setCharacterSize(100);
	levelCountT.setPosition(1650, 500);
	levelCountT.setFillColor(sf::Color::White);

	levelT.setFont(font);
	levelT.setCharacterSize(40);
	levelT.setPosition(1500, 450);
	levelT.setString("Level Count");

	// initialize buttons
	buttons.push_back(new UIButton("Assets/Menus/PlayGame.png", 380, 400, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/HighScores.png", 380, 600, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/Exit.png", 680, 800, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/Mul.png", 980, 400, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/options.png", 980, 600, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/plus.png", 1750, 500, 0.15f));
	buttons.push_back(new UIButton("Assets/Menus/minus.png", 1500, 500, 0.15f));
	buttons.push_back(new UIButton("Assets/Menus/Exit.png", 680, 800, 0.5f));

	//// initialize the main menu picture
	sf::Sprite& sprite = mng.CreateSprite("Laser", "Assets/Menus/fire.png");
	sprite.setPosition(720, 150);
	sprite.setScale(0.75, 0.75);

	if (!backGroundT.loadFromFile("Assets/Menus/menuBack.png"))
		errorMessageExit("Failed To Load Texture");
	backgroundS.setTexture(backGroundT);

	if (!clickB.loadFromFile("Assets/Audio/click.wav"))
		errorMessageExit("Sound Load Fail");

	clickS.setBuffer(clickB);
}

int MainMenu::Draw(sf::RenderWindow& _window, int totalLevels) {

	_window.draw(backgroundS);

	// check if any buttons are pressed and draw the buttons to screen
	if (buttons[0]->IsPressed()) {
		*menu = 5;
		clickS.play();
	}
	else if (buttons[1]->IsPressed())
		*menu = 2;
	else if (buttons[2]->IsPressed())
		*menu = 3;
	else if (buttons[3]->IsPressed())
		*menu = 4;
	else if (buttons[4]->IsPressed())
		*menu = 7;
	else if (buttons[5]->IsPressed() && totalLevels < 9)
		totalLevels++;
	else if (buttons[6]->IsPressed() && totalLevels > 1)
		totalLevels--;

	for (short i = 0; i < buttons.size(); i++)
		buttons[i]->DrawButton(_window);

	// draw the sprites to screen
	mng.DrawSprites(_window);
	// draw the title to screen
	_window.draw(laserEyesT);

	levelCountT.setString(to_string(totalLevels));
	_window.draw(levelCountT);
	_window.draw(levelT);

	return totalLevels;
}

void MainMenu::waitScreen(sf::RenderWindow& window, Network& networkT)//waiting screen
{
	window.draw(backgroundS);

	if (networkT.nO.waitTimer) {//shows count down until a process finishes, only used to connect to server
		waitingT.setString(networkT.nO.waitMessage + "\n\t" + to_string(networkT.nO.endTime - networkT.nO.currentTime) + " Seconds");

		buttons[7]->DrawButton(window);

		if (buttons[7]->IsPressed()) {//closes waiting screen
			*menu = 4;
			networkT.nO.killThreads();//kills runnning thread
		}
	}
	else
		waitingT.setString(networkT.nO.waitMessage);//used when thread is running, to avoid errors with shared memory

	window.draw(waitingT);
}

MainMenu::~MainMenu() {
	// delete the buttons from memory
	for (UIButton* b : buttons) {
		delete b;
	}
}