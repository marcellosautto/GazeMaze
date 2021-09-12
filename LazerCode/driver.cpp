#include "pch.h"
#include "driver.h"
#include "objDet.h"

SFMLLoader::SFMLLoader(int argc, const char** argv, std::array<int, 5> refreshArray)//constructor
{
	menu = refreshArray.at(0);//sets menu to the last menu 
	pmenu = refreshArray.at(1);//sets previous menu to last menu
	pollingKeyboard = textWait = update = VisitScore = pregameWait = refresh = exit = advance = false;
	keyboardIn = elpasedTime = STime = CTime = threadWaitPMenu = MazeTime = 0;
	nextFrameTime = 1000 / frameRate;//sets time till next frame

	if (!musicB.loadFromFile("Assets/Audio/Gaze_Maze.wav"))//loads music
		errorMessageExit("Sound Load Fail");
	musicS.setBuffer(musicB);

	//builds class objects with values from refresh array
	network = new Network;
	network->gO.totalLevels = refreshArray.at(2);
	obDet = new objDet(argc, argv, network->nO.waitScreen, network->nO.waitMessage, bool(refreshArray.at(3)));
	multiplayerScreen = new MultiplayerScreen(*network);
	gameInterface = new GameInterface(menu, obDet, *network);
	mainMenu = new MainMenu(menu, obDet, gameInterface);
	scoreScreen = new ScoreScreen(*network);
	controlScreen = new ControlScreen();

	if (!obDet->keyboard)//finds fps of camera on another thread
	{
		controlScreen->selected_txt.setPosition(225, 250);
		network->nO.waitScreen = true;
		thread sfpsThread(&objDet::setFPS, obDet, ref(obDet->movementsPerSecond));
		sfpsThread.detach();
	}
	else
		controlScreen->selected_txt.setPosition(1225, 250);

	wait = false;
}

SFMLLoader::~SFMLLoader()//deconstructor
{
	delete network;
	delete multiplayerScreen;
	delete obDet;
	delete gameInterface;
	delete mainMenu;
	delete scoreScreen;
	delete controlScreen;
}

std::array<int, 5> SFMLLoader::mainLoop(sf::RenderWindow& window) {//main loops runs program

	while (window.isOpen() && !refresh && !exit) { // This loop runs as long as the window stays open
		sf::Event event;
		std::vector<sf::Event> events; // a vector to store the events
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) // Checks if the window's close button was pressed
				window.close();
			else if (event.type == Event::GainedFocus)
				update = true;
			else if (event.type == Event::LostFocus)
				update = false;

			events.push_back(event); // pushes the events to the vector
		}
		elpasedTime = clock.getElapsedTime().asMilliseconds();// Elapsed time given by the clock

		if (elpasedTime >= nextFrameTime) {
			window.clear(sf::Color(0, 0, 0)); //Clear the screen

			if (!pollingKeyboard) {//checks keyboard input
				pollingKeyboard = true;

				if (event.type == sf::Event::TextEntered && !textWait) {
					keyboardIn = event.text.unicode;
					textWait = true;
				}
				else if (event.type == sf::Event::TextEntered)
					keyboardIn = 0;
				else
					textWait = false;

				if (keyboardIn == 27 && menu != 6)//escape was presssed
					if (menu == 0)//exit game, if alreay in the menu
						exit = true;
					else {// go to menu
						menu = 0;
						keyboardIn = 0;
						if (network->nO.multiplayerGame) {//if exit is pressed in multiplayer 
							network->nO.sendDisconnectSignal();//tells other user to diconnect
							refresh = true;
							pmenu = 0;
							break;
						}
					}

				pollingKeyboard = false;
			}

			if (network->nO.waitScreen && (menu == 5 || menu == 1 || menu == 4)) {//goes to waiting screen if waitscreen is enabled

				threadWaitPMenu = menu;
				menu = 6;
			}
			else if (!network->nO.waitScreen && menu == 6)//exits waiting screen, returns user to previous window, before wait screen was entered
				menu = threadWaitPMenu;

			if (network->nO.multiplayerGame) {//checks if other player has disconnected from the multiplayer game

				network->nO.killSocket.setBlocking(false);
				Packet killPacket;
				int checkForExit;
				network->nO.killSocket.receive(killPacket);
				killPacket >> checkForExit;
				if (checkForExit == 13) { //if exit packet was recieved, exit the program
					pmenu = 4;
					menu = 0;
				}
			}

			switch (menu) { // Draw the current menu
			case 0: // main menu
				if (pmenu == 4 || (pmenu == 2 && network->nO.multiplayerGame)) {//refreshes the game, if it was 
					pmenu = 0;
					refresh = true;
					break;
				}

				musicS.setLoop(false);
				musicS.stop();
				network->gO.totalLevels = mainMenu->Draw(window, network->gO.totalLevels);
				pmenu = 0;
				break;

			case 1: // play game menu

				pmenu = 1;
				if (gameInterface->Draw(window, elpasedTime, update, Start)) { //draws screen

					if (network->gO.levelsRemaining > 1) {//builds next level if there is still levels remaining
						network->gO.levelsRemaining--;
						delete gameInterface;
						gameInterface = new GameInterface(menu, obDet, *network);
					}
					else { //if there are no levels remaining 
						if (!network->nO.multiplayerGame) {//if single player
							duration <double> time_span = duration_cast<duration<double>>(chrono::steady_clock::now() - Start);
							MazeTime = time_span.count();
							scoreScreen->setMultiplayer(network->nO.multiplayerGame);//intializes variables in the score screen
							scoreScreen->setRecentLevel(network->gO.totalLevels);
							scoreScreen->setServerTime(MazeTime);
							menu = 2;
						}
						else if (network->nO.multiplayerGame) {//if multiplayer
							duration <double> time_span = duration_cast<duration<double>>(chrono::steady_clock::now() - Start);
							MazeTime = time_span.count();

							//intializes variables in the score screen
							scoreScreen->setServerTime(0);
							scoreScreen->setClientTime(0);
							if (network->nO.connectionType == 's')//for server
								scoreScreen->setServerTime(MazeTime);
							else if (network->nO.connectionType == 'c')//for client
								scoreScreen->setClientTime(MazeTime);
							scoreScreen->setLevel(network->gO.totalLevels);
							scoreScreen->setMultiplayer(network->nO.multiplayerGame);
							scoreScreen->UpdateScoreScreen();
							network->gO.levelsRemaining = network->gO.totalLevels;
							network->gO.totalLevels = scoreScreen->ScoreWindow(window, network->gO.totalLevels, menu, advance);
							menu = 2;//goes to score screen			
						}
					}
				}

				break;

			case 2: // high scores menu
				if (pmenu == 5) {//stops music
					musicS.stop();
					scoreScreen->trumpSound.play();
				}
				if (pmenu != 2) {//intializes score screen 
					while (!advance)
					{
						network->gO.totalLevels = scoreScreen->ScoreWindow(window, network->gO.totalLevels, menu, advance);
						break;
					}
					scoreScreen->setLevel(network->gO.totalLevels);
					scoreScreen->setMultiplayer(network->nO.multiplayerGame);
					if (network->nO.multiplayerGame) 
						scoreScreen->recordScore(MazeTime);
					scoreScreen->UpdateScoreScreen();
					pmenu = 2;
				}
				network->gO.totalLevels = scoreScreen->ScoreWindow(window, network->gO.totalLevels, menu, advance);

				break;

			case 3: // quit the game
				exit = true;

			case 4://multiplayer screen

				if (pmenu != 4) {//each time the multiplayer screen was entered
					refresh = true;
					pmenu = 4;
					break;
				}

				multiplayerScreen->Draw(window, menu, keyboardIn);
				pmenu = 4;
				break;

			case 5: // calibration screen
				if (!obDet->keyboard)
					obDet->runMotionDetect(gameInterface->userInput, window);//runs object detect to locate face
				else
					obDet->isCalibrated = true;

				if (obDet->isCalibrated) {// when the player has calibrated 

					if (controlScreen->resetCal)//resets calibration screen
					{
						menu = 0;
						controlScreen->resetCal = false;
					}
					else {//if calibrated and not reset move to the game
						menu = 1;
						if (network->nO.multiplayerGame)//multiplayer
							network->nO.waitScreen = true;
						else {//if singleplayer
							network->gO.levelsRemaining = network->gO.totalLevels;
							delete gameInterface;
							gameInterface = new GameInterface(menu, obDet, *network);

							musicS.play();
							musicS.setLoop(true);
							Start = steady_clock::now();
						}
					}
				}

				pmenu = 5;
				break;

			case 6://wait screen

				if (pregameWait) {		//counts down before the game begins
					if (network->nO.endTime - network->nO.currentTime < 1) {
						pregameWait = false;
						network->nO.waitScreen = false;
						network->nO.waitTimer = false;
						Start = steady_clock::now();
					}
					else
						time(&network->nO.currentTime);
				}
				else if (pmenu == 5) {
					network->nO.readySocket.setBlocking(false);

					if (network->nO.multiplayerGame && obDet->isCalibrated && !network->gO.multiplayerWait) {//sends signal to indicated if both players have started 
						network->nO.readyPacket << true;
						network->nO.readySocket.send(network->nO.readyPacket);
						network->gO.multiplayerWait = true;
					}

					bool rec = false; //checks if the other player has started the game
					network->nO.readySocket.receive(network->nO.readyPacket);
					network->nO.readyPacket >> rec;

					if (rec) {// if the other player has started the game 
						network->gO.multiplayerWait = false;
						network->gO.levelsRemaining = network->gO.totalLevels;
						delete gameInterface;
						gameInterface = new GameInterface(menu, obDet, *network);
						musicS.play();
						musicS.setLoop(true);
						pregameWait = true;
						network->nO.sendUsernames();
						network->nO.waitMessage = "Game Begins In:";
						network->nO.waitTimer = true;//begins countdown
						time(&network->nO.endTime);
						network->nO.endTime += 3;
						time(&network->nO.currentTime);
					}
					else {//shows waiting screen until other player is ready
						network->nO.waitScreen = true;
						network->nO.waitMessage = "Waiting For Other Player To Calibrate";
					}
				}

				mainMenu->waitScreen(window, *network);//draws waiting screen
				break;

			case 7://control screen
				controlScreen->draw(window, network->nO.waitScreen, menu, *obDet);//draws control screen
				pmenu = 7;
				break;

			case 8://motion control demo screen
				if (pmenu != 8)
					controlScreen->demo.open("Assets/Video/motion_demo.mp4");
				controlScreen->showDemo(window, menu);//shows demo video
				pmenu = 8;
				break;

			case 9://keyboard control demo screen
				if (pmenu != 9)
					controlScreen->demo.open("Assets/Video/keyboard_demo.mp4");
				controlScreen->showDemo(window, menu);//shows demo video
				pmenu = 9;
				break;

			default:
				break;
			}
			clock.restart();
		}
		window.display(); //Update the screen
	}
	if (!window.isOpen()) {//returns the program has been exited
		network->nO.sendDisconnectSignal();//tells other user to diconnect
		return { menu, pmenu, network->gO.totalLevels, int(obDet->keyboard), true };
	}
	//returns the current values of the program, so i can be reloaded
	return { menu, pmenu, network->gO.totalLevels, int(obDet->keyboard), exit };
}

int main(int argc, const char** argv)//launches loader
{
	mainLoader mL;
	mL.loader(argc, argv);

	return 0;
}

void mainLoader::loader(int argc, const char** argv)//loops until user exits the program
{
	while (!refreshArray.at(4)) {
		window.draw(backgroundS);//draws waiting screen while SFMLLoader is rebuilt
		window.draw(waitT);
		window.display();
		sL = new SFMLLoader(argc, argv, refreshArray);
		refreshArray = sL->mainLoop(window);
		delete sL;
	}
}

mainLoader::mainLoader()//constructor
{
	window.create(sf::VideoMode(1920, 1080), "Laser Eyes");//creates instance of a screen

	if (!backGroundT.loadFromFile("Assets/Menus/menuBack.png"))
		errorMessageExit("Failed To Load Texture");
	backgroundS.setTexture(backGroundT);

	refreshArray = { 0, 0, 3, 0, 0};

	if (!font.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");

	waitT.setFont(font);
	waitT.setCharacterSize(150);
	waitT.setPosition(500, 800);
	waitT.setString("Loading");
}
