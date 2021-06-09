#include "pch.h"
#include "menuScreen.h"

bool menuScreen::menuWindow()
{
	RenderWindow window(VideoMode(1920, 1080), PROG_NAME);

	window.setFramerateLimit(30);

	window.display();

	while (window.isOpen()) {

		while (window.pollEvent(windowEvent)) {

			if (windowEvent.type == Event::Closed)
				window.close();
		}
	}

	return false;
}