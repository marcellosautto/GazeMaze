#include "pch.h"
#include "GameInterface.h"

#define VIDEO_POS_X 1152
#define VIDEO_POS_Y 240

GameInterface::GameInterface(int& _menu, objDet* odt) {

	pG = new prepGame();
	od = odt;
	font.loadFromFile("Assets/Font/ponde___.ttf");
	videoTxt.setFont(font);
	videoTxt.setString("MOTION CAPTURE DISPLAY");
	videoTxt.setStyle(sf::Text::Style::Underlined);
	videoTxt.setCharacterSize(36);

}

void GameInterface::Draw(sf::RenderWindow& _window, float _elapsedTime) {

	input();
	drawMaze(_window);
	drawVideo(_window);
	p.playerS.DrawSprites(_window);
}


void GameInterface::drawMaze(sf::RenderTarget& renderTarget)
{
	for (int y = 0; y < pG->mazeS.size(); y++) {

		for (int x = 0; x < pG->mazeS.at(y).size(); x++) {

			pG->mazeS[y][x].setPosition((x + 1) * 50, (y + 1) * 50);
			renderTarget.draw(pG->mazeS[y][x]);
		}
	}
}

void GameInterface::drawVideo(sf::RenderTarget& renderTarget)
{
	od->videoSprite.setPosition(VIDEO_POS_X, VIDEO_POS_Y);
	videoTxt.setPosition(VIDEO_POS_X, VIDEO_POS_Y / 2);
	renderTarget.draw(od->videoSprite);
	renderTarget.draw(videoTxt);
}

// Removes the player physics from the list of global physics objects
//void GameInterface::RemovePlayerPhysics() {
//	Physics::Remove(player.pObj);
//}

bool GameInterface::checkCol(COORD newPos)
{
	if (newPos.Y / 50 - 1 >= 0 && newPos.X / 50 - 1 >= 0 &&
		pG->mazeV.at(newPos.Y / 50 - 1).at(newPos.X / 50 - 1) == '0')
		return true;

	return false;
}

void GameInterface::move(COORD newPos)
{
	if (checkCol(newPos)) {

		p.pos.Y = newPos.Y;
		p.pos.X = newPos.X;

		p.playerS.GetSprite("player").setPosition(p.pos.X, p.pos.Y);
	}
}

void GameInterface::input()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		exit(EXIT_SUCCESS);

	std::array<bool, 5> userInput = od->runMotionDetect();
	if (!userInput[4])
	{
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		if (userInput[0])
			move({ p.pos.X, short(p.pos.Y - 50) });

		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		if (userInput[1])
			move({ p.pos.X, short(p.pos.Y + 50) });

		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		if (userInput[2])
			move({ short(p.pos.X - 50), p.pos.Y });

		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		if (userInput[3])
			move({ short(p.pos.X + 50), p.pos.Y });
	}

}