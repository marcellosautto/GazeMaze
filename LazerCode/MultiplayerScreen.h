#pragma once
#include "pch.h"
#include "SpriteManager.h"
#include "Button.h"

class MultiplayerScreen {
private:
	sf::Texture backGroundT;
	sf::Sprite backgroundS;

	std::vector<UIButton*> buttons;

	int counter = 5;

	sf::Time connectTime;

	sf::Font font; 

	sf::Text connectedT, failedT, IPT, waitingT, waitingT2;

	string enterIP;
	ostringstream uipSS;
	void keyBoardIn(int in);

	int wait, ipSize, message[3] = {0};

public:

	MultiplayerScreen(sf::IpAddress ip);

	void Draw(sf::RenderWindow& window, sf::TcpListener& listener, string& Username, sf::TcpSocket& socket, sf::TcpSocket& socket2, sf::IpAddress& ip, sf::Packet& tempPacket, int& menu, int in, char &connectionType);
};