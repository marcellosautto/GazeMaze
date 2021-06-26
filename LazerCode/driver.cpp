#include "pch.h"
#include "driver.h"
#include "objDet.h"

void menuSwitch::SFMLLoader(int argc, const char** argv)
{
	char input;
	bool mul = false;
	IpAddress ip = IpAddress::getLocalAddress(); 
	TcpSocket socket;												// used to send map downloads and player movements
	TcpSocket socket2;												// used to send score times
	char buffer[2000];
	size_t received;
	char connectionType;
	Vector2f p2Position, prevPosition;
	string Username = " ";
	ScoreScreen sW;

	bool update = false, lastUserInputSelection = true;

	// Create
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Laser Eyes"); // Opens a window

	// Initialize the static classes
	HighScore::HighScore();

	// The current selected menu (menu) and whichever menu was selected last frame (pmenu)
	int menu = 0, pmenu = 0, levelCount = 0;
	bool preGameCheck = false;

	//Initialize Motion-Capture
	objDet od(argc, argv);

	// Initialize the game menus
	//ScoreScreen scoreScreen(menu);
	MainMenu mainMenu(menu);
	MultiplayerScreen MS(ip);

	GameInterface* gameInterface = new GameInterface(menu, &od, socket, p2Position, prevPosition, connectionType, mul);

	if (!gameInterface->keyboard)
	{
		mainMenu.alreadySelectedTxt.setPosition(1300, 800);
	}

	else
		mainMenu.alreadySelectedTxt.setPosition(1300, 900);

	Packet tempPacket;
	TcpListener listener;

	int Player2Time = 0;
	int STime = 0;
	int CTime = 0;
	bool temp = false;
	//Packet packet;
	// Initialize the game clock
	sf::Clock clock;
	float nextFrameTime = 1000 / frameRate;
	bool test = true;
	int Level;
	auto start = chrono::steady_clock::now();
	while (window.isOpen()) { // This loop runs as long as the window stays open
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

		// Elapsed time given by the clock
		float elpasedTime = clock.getElapsedTime().asMilliseconds();

		if (elpasedTime >= nextFrameTime) {
			window.clear(sf::Color(0, 0, 0)); //Clear the screen

			switch (menu) { // Draw the current menu
			case 0: // main menu
				levelCount = mainMenu.Draw(window, event.text.unicode);
				Level = levelCount;
				break;
			case 1: // play game menu
				if (test)
				{
					test = false;
					gameInterface = new GameInterface(menu, &od, socket, p2Position, prevPosition, connectionType, mul);
				}
				if (pmenu != 1)
				{
					/*socket.setBlocking(true);
					packet << true;
					socket.send(packet);
					socket.receive(packet);
					packet >> temp;
					if (temp)
					{
						gameInterface = NULL;
						delete gameInterface;
						gameInterface = new GameInterface(menu, &od, socket, p2Position, prevPosition, connectionType, mul);
					}*/
				}
				pmenu = 1;
				if (gameInterface->Draw(window, elpasedTime, update)) 
				{
					if (levelCount > 1)
						gameInterface = new GameInterface(menu, &od, socket, p2Position, prevPosition, connectionType, mul);
					levelCount--;
					if (levelCount == 0)
					{
						auto end = chrono::steady_clock::now();
						int MazeTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
						int wholeNumber = MazeTime / 1000;
						int decimal = MazeTime % 1000;
						cout << "Elapsed time in seconds: "
							<< wholeNumber << "." << decimal
							<< " s" << endl;
						char Tbuffer[2000];
						size_t Treceived;
						string text = to_string(MazeTime);
						socket2.send(text.c_str(), text.length() + 1);
						socket2.setBlocking(true);
						socket2.receive(Tbuffer, sizeof(Tbuffer), Treceived);
						string::size_type sz;   // alias of size_t
						if (mul) {
							int Player2Time = stoi(Tbuffer, &sz);
							wholeNumber = Player2Time / 1000;
							decimal = Player2Time % 1000;
							cout << "Elapsed opponent time in seconds: "
								<< wholeNumber << "." << decimal
								<< " s" << endl;
							if (connectionType == 's')
							{
								STime = MazeTime;
								CTime = Player2Time;
							}
							else
							{
								CTime = MazeTime;
								STime = Player2Time;
							}

						}
						if (!mul)
							STime = MazeTime; // This is for single player mode, sends STime as the maze time score
						menu = 2;
					}
				}
				break;
			case 2: // high scores menu
				sW.setServerTime(STime);
				sW.setLevel(Level);
				sW.setClientTime(CTime);
				sW.setMultiplayer(mul);
				sW.EndGameTime();

				do
				{
					menu = sW.ScoreWindow(window);
				} while (menu == -1);

				//sW.ScoreWindow(window);
				break;

			case 3: // quit the game
				exit(EXIT_SUCCESS);


			case 4: // Multiplayer screen
				pmenu = 4;
				mul = true;
				MS.Draw(window,listener,Username,socket,socket2, ip,tempPacket, menu, event.text.unicode, connectionType);
				break;

			case 5:

				// Is this new? Was this deleted?
				//if (gameInterface->keyboard != lastUserInputSelection) //if we're not using the keyboard and the last user selection was not true
				//{
				//	lastUserInputSelection = od.cameraSwitch(gameInterface->keyboard);
				//	gameInterface->keyboard = lastUserInputSelection;
				//}
				if (!gameInterface->keyboard)
					preGameCheck = od.calibrate(window);
				else
					preGameCheck = true;

				if (preGameCheck)
					menu = 1;
				//if (pmenu == 5)
				//{
				//	gameInterface = new GameInterface(menu, &od, socket, p2Position, prevPosition, connectionType, mul);
				//}
				break;
			case 6:
				if (gameInterface->keyboard != false) //switch to motion controls
					gameInterface->keyboard = od.cameraSwitch(false);

				mainMenu.alreadySelectedTxt.setPosition(1300, 800);
				menu = 0;
				break;

			case 7:
				if (gameInterface->keyboard != true) //switch to keyboard controls
					gameInterface->keyboard = od.cameraSwitch(true);

				mainMenu.alreadySelectedTxt.setPosition(1300, 900);
				menu = 0;
				break;
			default:
				break;
			}
			// set the previous menu
			if (pmenu == 4 && menu == 1)
				gameInterface = new GameInterface(menu, &od, socket, p2Position, prevPosition, connectionType, mul);
			//else if (pmenu == 2 && menu == 0)
			pmenu = menu;
			// reset the clock
			clock.restart();
		}
		window.display(); //Update the screen
	}
}

int main(int argc, const char** argv)
{
	menuSwitch mS;
	mS.SFMLLoader(argc, argv);

	return 0;
}