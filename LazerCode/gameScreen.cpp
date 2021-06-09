#include "pch.h"
#include "gameScreen.h"


bool gameScreen::gameWindow()
{
	RenderWindow window(VideoMode(1920, 1080), PROG_NAME);

	window.setFramerateLimit(30);

	window.draw(pG.backgroundSprite);
	drawMaze(window);
	window.display();

	while (window.isOpen()) {

		while (window.pollEvent(windowEvent)) {

			if (windowEvent.type == Event::Closed)
				window.close();
		}
	}

	return false;
}

void gameScreen::drawMaze(RenderTarget& renderTarget){

	//goes through every point of maveS vector and draws each sprite in correct position
	for (int y = 0; y < pG.mazeS.size(); y++) {

		for (int x = 0; x < pG.mazeS.at(y).size(); x++) {

			pG.mazeS[y][x].setPosition((x + 1) * 50, (y + 1) * 50);
			renderTarget.draw(pG.mazeS[y][x]);
		}
	}
}
