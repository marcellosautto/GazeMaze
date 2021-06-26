#include "pch.h"
#include "GameInterface.h"

GameInterface::GameInterface(int& _menu, objDet* odt, TcpSocket& TempSocket, Vector2f& Tempp2Position, Vector2f& TempprevPosition, char& TempconnectionType, bool tmul) {

	mul = tmul;
	pG = new prepGame(TempconnectionType, TempSocket, mul);
	//p1.playerAni = new animate({ 100.0f,50.0f });
	//p1.playerAni->update(1);
	od = odt;

	int camera_device = 0;
	odt->setFPS(camera_device);
	odt->capture.open(camera_device);
	keyboard = !(odt->capture.isOpened());



	userInput = { false };

	if (mul) {
		socket = &TempSocket;
		p2Position = &Tempp2Position;
		prevPosition = &TempprevPosition;
	}


}

bool GameInterface::Draw(sf::RenderWindow& _window, float _elapsedTime, bool update) {

	_window.draw(pG->backgroundS);

	if (mul) {
		Packet packet;
		*prevPosition = p1.playerSprite.GetSprite("playerSprite").getPosition();


		if (update)
			if (input())
				return true;

		if (*prevPosition != p1.playerSprite.GetSprite("playerSprite").getPosition())
		{
			packet << p1.playerSprite.GetSprite("playerSprite").getPosition().x << p1.playerSprite.GetSprite("playerSprite").getPosition().y;
			socket->send(packet);
		}
		socket->receive(packet);
		if (packet >> p2Position->x >> p2Position->y)
			p2.playerSprite.GetSprite("playerSprite").setPosition(*p2Position);

	}
	else
		if (input())
			return true;

	if (userInput[4])
		_window.draw(pG->oTxt);

	else if (userInput[0])
		_window.draw(pG->upTxt);

	else if (userInput[1])
		_window.draw(pG->downTxt);

	else if (userInput[2])
		_window.draw(pG->leftTxt);

	else if (userInput[3])
		_window.draw(pG->rightTxt);


	drawMaze(_window);
	p1.playerSprite.DrawSprites(_window);
	if (mul)
		p2.playerSprite.DrawSprites(_window);

	//p1.playerAni->update(_elapsedTime);
	//p1.playerAni->setdriection(p1.playerVec);
	//p1.playerAni->draw(_window);
	if (!keyboard)
		drawVideo(_window);

	return false;
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
	od->videoSprite.setPosition(1150, 240);
	renderTarget.draw(od->videoSprite);
	renderTarget.draw(pG->videoTxt);
}

bool GameInterface::checkCol(COORD newPos)
{
	if (newPos.Y / 50 - 1 >= 0 && newPos.X / 50 - 1 >= 0 &&
		pG->mazeV.at(newPos.Y / 50 - 1).at(newPos.X / 50 - 1) == '0')
		return true;

	return false;
}

bool GameInterface::move(COORD newPos)
{
	if (newPos.Y / 50 > 19)
		return true;

	else if (checkCol(newPos)) {

		p1.pos.Y = newPos.Y;
		p1.pos.X = newPos.X;

		//p1.playerVec.x = float(newPos.X);
		//p1.playerVec.y = float(newPos.Y);

		p1.playerSprite.GetSprite("playerSprite").setPosition(p1.pos.X, p1.pos.Y);
	}

	return false;
}

bool GameInterface::input()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		exit(EXIT_SUCCESS);

	if (!keyboard)
	{
		userInput = od->runMotionDetect(userInput);
		if (!userInput[4] && od->motionDetectF.size() == 0)
		{
			if (userInput[0])
				if (move({ p1.pos.X, short(p1.pos.Y - 50) }))
					return true;

			if (userInput[1])
				if (move({ p1.pos.X, short(p1.pos.Y + 50) }))
					return true;

			if (userInput[2])
				if (move({ short(p1.pos.X - 50), p1.pos.Y }))
					return true;

			if (userInput[3])
				if (move({ short(p1.pos.X + 50), p1.pos.Y }))
					return true;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		keyboard = !keyboard;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		if (move({ p1.pos.X, short(p1.pos.Y - 50) }))
			return true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		if (move({ p1.pos.X, short(p1.pos.Y + 50) }))
			return true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		if (move({ short(p1.pos.X - 50), p1.pos.Y }))
			return true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		if (move({ short(p1.pos.X + 50), p1.pos.Y }))
			return true;

	return false;
}