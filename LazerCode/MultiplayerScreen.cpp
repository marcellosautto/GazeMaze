#include "pch.h"
#include "MultiplayerScreen.h"

MultiplayerScreen::MultiplayerScreen(Network& networkT)//constructor
{
	showIP = false;
	if (!backGroundT.loadFromFile("Assets/Menus/menuBack.png"))
		errorMessageExit("Failed To Load Texture");

	if (!font.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");
	if (!font2.loadFromFile("Assets/Font/Calibri 400.ttf"))
		errorMessageExit("Failed To Load Font");

	network = &networkT;
	uipSS << network->nO.userIP;//sets ip

	backgroundS.setTexture(backGroundT);
	//creates buttons
	buttons.push_back(new UIButton("Assets/Menus/local.png", 700, 850, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/Host.png", 150, 850, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/client.png", 1250, 850, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/start.png", 150, 650, 0.5));
	buttons.push_back(new UIButton("Assets/Menus/BackToMenu.png", 1250, 650, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/plus.png", 1750, 500, 0.15f));
	buttons.push_back(new UIButton("Assets/Menus/minus.png", 1500, 500, 0.15f));
	buttons.push_back(new UIButton("Assets/Menus/hide_ip.png", 700, 650, 0.5f));

	//builds sfml text
	levelCountT.setFont(font);
	levelCountT.setCharacterSize(100);
	levelCountT.setPosition(1650, 500);
	levelCountT.setFillColor(sf::Color::White);

	IPT.setFont(font);
	IPT.setCharacterSize(100);
	IPT.setPosition(10, 50);

	connectedT.setFont(font);
	connectedT.setCharacterSize(100);
	connectedT.setPosition(100, 350);
	connectedT.setString("Connected!");

	failedT.setFont(font);
	failedT.setCharacterSize(100);
	failedT.setPosition(100, 350);
	failedT.setString("Connection Failed!");

	waitingT.setFont(font);
	waitingT.setCharacterSize(84);
	waitingT.setPosition(100, 350);
	waitingT.setString("Waiting For Other Player To Press Play");

	localIPT.setFont(font);
	localIPT.setCharacterSize(100);
	localIPT.setPosition(10, 200);
	localIPT.setString("Your IP:" + network->nO.userIP.toString());

	startFailT.setFont(font);
	startFailT.setCharacterSize(84);
	startFailT.setPosition(100, 350);
	startFailT.setString("Connect To Another Player, \n\t Then Press Start");

	blink.setFont(font);
	blink.setCharacterSize(100);
	blink.setString("_");

	ipCover.setSize(sf::Vector2f(150, 100));

	levelT.setFont(font);
	levelT.setCharacterSize(40);
	levelT.setPosition(1500, 450);
	levelT.setString("Level Count");

	blinkTime = 0;
	lastKey = 0;
}

void MultiplayerScreen::Draw(sf::RenderWindow& window, int& menu, int in)
{
	window.draw(backgroundS);

	keyBoardIn(in);//takes keyboard input

	if (buttons[0]->IsPressed())//search for local host
		network->nO.prepLocalHostConnect();

	else if (buttons[1]->IsPressed())//hosts server
		network->nO.prepHostConnect();

	else if (buttons[2]->IsPressed())//join server entered in screen
		network->nO.prepclientConnect(uipSS.str());

	else if (buttons[3]->IsPressed() && network->nO.connectionType != ' ') {//if start is pressed and player has successsfully connected to another
		network->nO.startMultiplayer();
		menu = 5;
	}
	else if (buttons[3]->IsPressed())//start is pressed and player is not connected to another
		network->nO.msMessage[3] = 600;

	else if (buttons[4]->IsPressed())//exit is pressed
		menu = 0;
	else if (buttons[5]->IsPressed() && network->gO.totalLevels < 9)//increase levels
		network->gO.totalLevels++;
	else if (buttons[6]->IsPressed() && network->gO.totalLevels > 1)//cecrease levels
		network->gO.totalLevels--;
	else if (buttons[7]->IsPressed()) //show/hide ip on screen
		showIP = !showIP;

	buttons[0]->DrawButton(window);//draws buttons on screeb
	buttons[1]->DrawButton(window);
	buttons[2]->DrawButton(window);
	buttons[3]->DrawButton(window);
	buttons[4]->DrawButton(window);
	buttons[5]->DrawButton(window);
	buttons[6]->DrawButton(window);
	buttons[7]->DrawButton(window);

	//shows messages for 600 frames
	if (network->nO.msMessage[0] > 0) {
		window.draw(connectedT);
		network->nO.msMessage[0]--;
	}
	else if (network->nO.msMessage[1] > 0) {
		window.draw(failedT);
		network->nO.msMessage[1]--;
	}
	else if (network->nO.msMessage[2] > 1) {
		window.draw(waitingT);
		network->nO.msMessage[2]--;
	}
	else if (network->nO.msMessage[3] > 1) {
		window.draw(startFailT);
		network->nO.msMessage[3]--;
	}

	IPT.setString("Host IP:" + uipSS.str());

	if (time(&blinkTime) % 2) {//shows blinking line under ip
		blink.setPosition(uipSS.str().size() * 81 + 600, 75);
		window.draw(blink);
	}

	levelCountT.setString(to_string(network->gO.totalLevels));
	window.draw(levelCountT);
	window.draw(levelT);
	window.draw(localIPT);
	window.draw(IPT);

	if (!showIP) {//shows ip or hides it with rectangle sprites
		if (uipSS.str().size() > 0)
			for (int i = 0; i < uipSS.str().size() - 1; i++) {

				ipCover.setPosition(i * 81 + 600, 50);
				window.draw(ipCover);
			}

		for (int i = 0; i < network->nO.userIP.toString().size(); i++) {

			ipCover.setPosition(i * 81 + 600, 200);
			window.draw(ipCover);
		}
	}
}

void MultiplayerScreen::keyBoardIn(int in)//determines keyboard input and runs functions
{
	if (lastKey != in) {
		lastKey = in;
		if (in == 8 && uipSS.str().size() > 0) {//if backspace was pressed delete last number input
			string temp = uipSS.str();
			temp.pop_back();
			uipSS.clear();
			uipSS.str("");
			uipSS << temp;
		}

		else if ((in < 58) && (in > 47) && uipSS.str().size() < 15) //If number is entered
			uipSS << in - 48;

		else if (in == 46 && uipSS.str().size() < 15)//if period was entered
			uipSS << '.';
	}
}