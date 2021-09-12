#include "pch.h"
#include "GameInterface.h"

GameInterface::GameInterface(int& _menu, objDet* odt, Network& networkT) {//constructor

	network = &networkT;
	network->nO.multiplayerGame;
	pG = new prepGame(*network);//build assets for the game
	od = odt;

	p1.playerAni = new animate(p1.p_move, false);//creates new animation
	p1.playerAni->update(1);

	userInput = { false };

	if (network->nO.multiplayerGame) {//if multiplayer game, create animation for player 2
		p2.playerAni = new animate(p2.p_move, true);
		p2.playerAni->update(1);
	}
	newLevel = completed = false;
}

bool GameInterface::Draw(sf::RenderWindow& _window, float _elapsedTime, bool update, steady_clock::time_point t1) {//draws player on screen and refreshs frame

	_window.draw(pG->backgroundS);//draws background
	drawMaze(_window);

	if (network->nO.multiplayerGame) {//sends and recieves player movement and which level the player is on

		if (update &! completed)
			if (input(_window))
				return true;

		if (completed) {// when the game is finished, move the player off of the screen
			p1.p_move = { 4000, 200 };
			p1.pos = { 4000, 200 };
		}

		Packet p2LevelPacket;
		int p2Level;
		p2LevelPacket << network->gO.currentLevel();//sends and recieves the current level
		network->nO.p2LevelSocket.send(p2LevelPacket);
		network->nO.p2LevelSocket.receive(p2LevelPacket);
		p2LevelPacket >> p2Level;

		if (p2Level == network->gO.currentLevel()) {//if the two players on the same level, draw them both on the screen

			Packet packet;

			if (!newLevel) {//if the player has just begun a new level, draw player 2 on the screen

				packet << int(p1.pos.X) << int(p1.pos.Y);
				network->nO.playerSocket.send(packet);
				newLevel = true;
			}
			else if (network->gO.p1PrevPosition.x != p1.pos.X || network->gO.p1PrevPosition.y != p1.pos.Y)
			{//if the player has moved, update the position
				packet << int(p1.pos.X) << int(p1.pos.Y);
				network->nO.playerSocket.send(packet);
			}

			int x = 0;
			int y = 0;

			network->nO.playerSocket.receive(packet);
			packet >> x >> y;
			//updates the position of the player 2
			if ((network->gO.p2PrevPosition.x != x || network->gO.p2PrevPosition.y != y) && x >= 50 && y >= 50) {
				network->gO.p2PrevPosition.x = x;
				network->gO.p2PrevPosition.y = y;
				p2.p_move.x = float(network->gO.p2PrevPosition.x);
				p2.p_move.y = float(network->gO.p2PrevPosition.y);
				p2.playerSprite.GetSprite("playerSprite").setPosition(network->gO.p2PrevPosition.x, network->gO.p2PrevPosition.y);
				p2.pos = { short(network->gO.p2PrevPosition.x) , short(network->gO.p2PrevPosition.y) };
				p2.playerAni->move_player(p2.p_move);
			}

			p2.playerAni->update(_elapsedTime);
			p2.playerAni->draw(_window);
		}
		else
			newLevel = false;
		//updates players previous position
		network->gO.p1PrevPosition.x = p1.pos.X;
		network->gO.p1PrevPosition.y = p1.pos.Y;
	}
	else
		if (input(_window))//if the game is single player, check if user has updated the characters position
			return true;

	if (userInput[4])//shows direction of users movement
		_window.draw(pG->oTxt);
	else if (userInput[0])
		_window.draw(pG->upTxt);
	else if (userInput[1])
		_window.draw(pG->downTxt);
	else if (userInput[2])
		_window.draw(pG->leftTxt);
	else if (userInput[3])
		_window.draw(pG->rightTxt);

	p1.playerAni->move_player(p1.p_move);//updates sprite and draws it on screen
	p1.playerAni->update(_elapsedTime);
	p1.playerAni->draw(_window);

	if (!od->keyboard)//if the camera is the input type, draw it on the screen
		drawVideo(_window);

	pG->levelTxt.setString("Current Level: " + to_string(network->gO.currentLevel() + 1));//display the current level
	_window.draw(pG->levelTxt);

	drawClock(_window, t1);//draws the clock on the screen

	return false;
}

void GameInterface::drawClock(sf::RenderTarget& renderTarget, steady_clock::time_point t1)//draws clock on the screen
{
	steady_clock::time_point t2 = steady_clock::now();
	duration <double> time_span = duration_cast<duration<double>>(t2 - t1);
	pG->timeTxt.setString("Timer: " + to_string(int(time_span.count())) + " Seconds");
	renderTarget.draw(pG->timeTxt);
}

void GameInterface::drawMaze(sf::RenderTarget& renderTarget)//draw maze on the screen
{
	for (int y = 0; y < pG->mazeS.size(); y++)
		for (int x = 0; x < pG->mazeS.at(y).size(); x++) {
			pG->mazeS[y][x].setPosition((x + 1) * 50, (y + 1) * 50);
			renderTarget.draw(pG->mazeS[y][x]);
		}
}

void GameInterface::drawVideo(sf::RenderTarget& renderTarget)//draws video onto the screen
{
	od->videoSprite.setPosition(1150, 450);
	renderTarget.draw(od->videoSprite);
	renderTarget.draw(pG->videoTxt);
}

bool GameInterface::checkCol(COORD newPos)//check if player will colide with a wall
{
	newPos.Y += newPos.Y % 50;
	newPos.X += newPos.X % 50;

	if (newPos.Y / 50 - 1 >= 0 && newPos.X / 50 - 1 >= 0)
		if (pG->mazeV.at(newPos.Y / 50 - 1).at(newPos.X / 50 - 1) == '0')
			return true;

	return false;
}

bool GameInterface::move(COORD newPos)//moves player if it does not colide with a wall
{
	if (newPos.Y / 50 > 18)
		return true;

	if (checkCol(newPos)) {

		p1.pos.Y = newPos.Y;
		p1.pos.X = newPos.X;

		p1.p_move.x = float(p1.pos.X);
		p1.p_move.y = float(p1.pos.Y);
	}

	return false;
}

bool GameInterface::input(sf::RenderWindow& _window)//checks/interpretes direction user wants to move the player
{
	if (!od->keyboard)//for camera controls
	{
		userInput = od->runMotionDetect(userInput, _window);
		if (!userInput[4] && od->motionDetectF.size() == 0)
		{
			if (userInput[0])
				if (move({ p1.pos.X, short(p1.pos.Y - 10) }))
					return true;

			if (userInput[1])
				if (move({ p1.pos.X, short(p1.pos.Y + 10) }))
					return true;

			if (userInput[2])
				if (move({ short(p1.pos.X - 10), p1.pos.Y }))
					return true;

			if (userInput[3])
				if (move({ short(p1.pos.X + 10), p1.pos.Y }))
					return true;
		}
	}
	else {//for keyboard controls
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			if (move({ p1.pos.X, short(p1.pos.Y - 1) }))
				return true;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			if (move({ p1.pos.X, short(p1.pos.Y + 1) }))
				return true;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			if (move({ short(p1.pos.X - 1), p1.pos.Y }))
				return true;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			if (move({ short(p1.pos.X + 1), p1.pos.Y }))
				return true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		if (move({ 18 * 50, 18 * 50 }))
			return true;

	return false;
}