#include "pch.h"
#include "driver.h"
#include "objDet.h"

void menuSwitch::SFMLLoader(int argc, const char** argv){
	// Create
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Laser Eyes"); // Opens a window

	// Initialize the static classes
	HighScore::HighScore();
	Physics::Physics();

	// The current selected menu (menu) and whichever menu was selected last frame (pmenu)
	int menu = 0;
	int pmenu = 0;

	//Initialize Motion-Capture
	objDet od (argc, argv);

	// Initialize the game menus
	ScoreScreen scoreScreen(menu);
	MainMenu mainMenu(menu);
	GameInterface gameInterface(menu, &od);


	About about(menu);

	// Initialize the game clock
	sf::Clock clock;
	float nextFrameTime = 1000 / frameRate;

	

	while (window.isOpen()) { // This loop runs as long as the window stays open
		sf::Event event;
		std::vector<sf::Event> events; // a vector to store the events
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) // Checks if the window's close button was pressed
				window.close();
			events.push_back(event); // pushes the events to the vector
		}

		// Elapsed time given by the clock
		float elpasedTime = clock.getElapsedTime().asMilliseconds();


		if (elpasedTime >= nextFrameTime) {
			window.clear(sf::Color(0, 0, 0)); //Clear the screen

			switch (menu) { // Draw the current menu
			case 0: // main menu
				mainMenu.Draw(window);
				break;
			case 1: // play game menu
				window.clear(sf::Color(70, 130, 255));
				gameInterface.Draw(window, elpasedTime);
				break;
			case 2: // high scores menu
				/*if (HighScore::askForInitials)
					scoreScreen.AskForInitials(window, events);*/
				scoreScreen.Draw(window);
				break;
				//case 3: // draw the about menu
				//	window.clear(sf::Color::Black);
				//	about.Draw(window);
				//	break;
				case 3: // quit the game
					exit(EXIT_SUCCESS);
				default:
					break;
				}


			// set the previous menu
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
	//od.runMotionDetect();

	return 0;
}