#include "pch.h"
#include "MultiplayerScreen.h"


MultiplayerScreen::MultiplayerScreen(sf::IpAddress ip)
{
	if (!backGroundT.loadFromFile("Assets/Menus/menuBack.png"))
		errorMessageExit("Failed To Load Texture");

	if (!font.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");

	backgroundS.setTexture(backGroundT);

	buttons.push_back(new UIButton("Assets/Menus/local.png", 610, 600, 2.5));
	buttons.push_back(new UIButton("Assets/Menus/Host.png", 610, 700, 2.5));
	buttons.push_back(new UIButton("Assets/Menus/client.png", 610, 800, 2.5));
	buttons.push_back(new UIButton("Assets/Menus/start.png", 610, 900, 2.5));
	counter = 5;

	IPT.setFont(font);
	IPT.setCharacterSize(100);
	IPT.setPosition(100, 100);

	connectedT.setFont(font);
	connectedT.setCharacterSize(100);
	connectedT.setPosition(100, 300);
	connectedT.setString("Connected!");

	failedT.setFont(font);
	failedT.setCharacterSize(100);
	failedT.setPosition(100, 300);
	failedT.setString("Connection Failed!");

	waitingT.setFont(font);
	waitingT.setCharacterSize(100);
	waitingT.setPosition(100, 300);
	waitingT.setString("Waiting For Other Player To Press Play");

	waitingT.setFont(font);
	waitingT.setCharacterSize(100);
	waitingT.setPosition(100, 300);
	waitingT.setString("Waiting For Connection");

	uipSS << ip;

	string ipsizes = ip.toString();
	ipSize = ipsizes.size();
	wait = 0;

}

void MultiplayerScreen::Draw(sf::RenderWindow& window, sf::TcpListener& listener, string& Username, sf::TcpSocket& socket, sf::TcpSocket& socket2, sf::IpAddress& ip, sf::Packet& tempPacket,int& menu, int in, char &connectionType)
{
	window.draw(backgroundS);

	if (wait == 0) {
		keyBoardIn(in);
		wait = 22;
	}
	else if (wait > 0)
		wait--;

	if (buttons[0]->IsPressed()) {
	
		window.draw(waitingT);
		window.display();
		std::string ipS = ip.toString();

		ipS.pop_back();
		ipS.pop_back();
		//ipS.pop_back();

		for (int i = 0; i < 1000; i++) {
			ostringstream ipSS;
			
			if(i > 99)
				ipSS << ipS << i;
			else if (i > 9)
				ipSS << ipS << "0" << i;
			else
				ipSS << ipS << "00" << i;

			cout << ipSS.str() << endl;

			ip = ipSS.str();
			sf::Socket::Status status = socket.connect(ip,2000, sf::milliseconds(10));
			if (status == sf::Socket::Done) {
				Username = "Client";
				message[0] = 600;
				connectionType = 'c';
				break;
			}
			socket2.connect(ip, 2001);
			
			if(i == 999)
				message[1] = 600;
		}
	}
	if (buttons[1]->IsPressed()) {
		window.draw(waitingT);
		window.display();
		listener.listen(2000);
		sf::Socket::Status status = listener.accept(socket);
		if (status == sf::Socket::Done) {
			//  Can ask user for Username, will be used to display score on scoreboard
			Username = "Server host";
			message[0] = 600;
			connectionType = 's';
		}
		else
			message[1] = 600;
		listener.listen(2001);
		listener.accept(socket2);

	}
	if (buttons[2]->IsPressed()) {

		ip = uipSS.str();
		window.draw(waitingT);
		window.display();
		sf::Socket::Status status = socket.connect(ip, 2000, sf::milliseconds(5000));
		if (status == sf::Socket::Done) {
			Username = "Client";
			message[0] = 600;
			connectionType = 'c';
		}
		else 
			message[1] = 600;
		socket2.connect(ip, 2001);
		//  Can ask user for Username, will be used to display score on scoreboard
	}
	if (buttons[3]->IsPressed() && Username != " ") {
		message[2] = 600;
		socket.setBlocking(true);
		tempPacket << true;
		socket.send(tempPacket);
		socket.receive(tempPacket);
		socket.setBlocking(false);
		menu = 5;
	}
	buttons[0]->DrawButton(window);
	buttons[1]->DrawButton(window);
	buttons[2]->DrawButton(window);
	buttons[3]->DrawButton(window);

	if (message[0] > 0) {
		window.draw(connectedT);
		message[0]--;
	}
	else if (message[1] > 0) {
		window.draw(failedT);
		message[1]--;
	}
	else if (message[2] > 0) {
		window.draw(waitingT);
		message[2]--;
	}

	IPT.setString(uipSS.str());
	window.draw(IPT);
}


void MultiplayerScreen::keyBoardIn(int in)//determines keyboard input and runs functions
{
	if (in == 8) {//if backspace was pressed delete last number input
		string temp = uipSS.str();
		temp.pop_back();
		uipSS.clear();
		uipSS.str("");
		uipSS << temp;
	}

	else if ((in < 58) && (in > 47) && uipSS.str().size() < ipSize) //If number is entered
		uipSS << in - 48;
	
}
