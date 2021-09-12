#include "pch.h"
#include "network.h"

Network::networkObjects::networkObjects()
{
	userIP = IpAddress::getLocalAddress();
	connectionType = ' ';
	clientUsername = waitMessage = " ";
	waitScreen = waitTimer = multiplayerGame = killThread = false;

	TCHAR username[UNLEN + 1];//finds username of player
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);

	for (int i = 0; i < size - 1; i++)
		hostUsername.push_back(username[i]);
}

Network::networkObjects::~networkObjects()//closes all listeners and disconnects all sockets
{
	killListener.close();
	playerListener.close();
	scoreListener.close();
	mapListener.close();
	readyListener.close();
	p2LevelListener.close();

	killSocket.disconnect();
	playerSocket.disconnect();
	scoreSocket.disconnect();
	mapSocket.disconnect();
	readySocket.disconnect();
	p2LevelSocket.disconnect();
}

int Network::gameObjects::currentLevel()//returns current level
{
	return totalLevels - levelsRemaining;
}

void Network::networkObjects::sendDisconnectSignal()//signals to other player, that the player has disconnected
{
	if (connectionType != ' ') {
		Packet killPacket;
		killPacket << 13;
		killSocket.send(killPacket);
	}
}

void Network::networkObjects::sendUsernames()//sends and recieves username to other player
{
	char Tbuffer[2000];
	size_t Treceived;
	string temp;
	string text = temp = hostUsername;
	scoreSocket.send(text.c_str(), text.length() + 1);
	scoreSocket.setBlocking(true);
	scoreSocket.receive(Tbuffer, sizeof(Tbuffer), Treceived);
	if (connectionType == 's')
	{
		clientUsername = Tbuffer;
	}
	else if (connectionType == 'c')
	{
		hostUsername = Tbuffer;
		clientUsername = temp;
	}
}

Network::gameObjects::gameObjects() {//constructor

	multiplayerWait = false;
	Player2Time = levelsRemaining = 0;
	totalLevels = 0;
	p2PrevPosition = { 100,50 };
};

void Network::networkObjects::prepclientConnect(string userIPT)//code run before client connect is launched on its own thread
{
	killThread = false;
	userIP = userIPT;//sets ip
	time(&currentTime);//intializes time
	time(&endTime);
	endTime += 10;//sets time until thread is killed
	playerListener.setBlocking(true);
	waitScreen = true;
	waitTimer = true;
	acThreads[2] = std::thread(&Network::networkObjects::clientConnect, this, ref(waitScreen), ref(killSocket), ref(playerSocket), ref(scoreSocket), ref(mapSocket), ref(readySocket), ref(p2LevelSocket), ref(connectionType), ref(msMessage), ref(waitTimer));
	acThreads[2].detach();
	waitMessage = "\tAttempting To Join Host\t\nApproximate Time Remaining: ";
}

void Network::networkObjects::prepLocalHostConnect()// code run before local host connect is launched on its own thread
{
	killThread = false;
	time(&currentTime);
	time(&endTime);//intializes time
	endTime += 30;//sets time until thread is killed
	waitScreen = true;
	waitTimer = true;
	acThreads[1] = std::thread(&Network::networkObjects::findLocalHost, this, ref(waitScreen), ref(killSocket), ref(playerSocket), ref(scoreSocket), ref(mapSocket), ref(readySocket), ref(p2LevelSocket), ref(connectionType), ref(msMessage), ref(waitTimer));
	acThreads[1].detach();
	waitMessage = "\tSearching For Local Host\t\nApproximate Time Remaining: ";
}

void Network::networkObjects::prepHostConnect()//code run before host connect is launched on its own thread
{
	playerListener.listen(2000);//prepares listener
	scoreListener.listen(2001);
	mapListener.listen(2002);
	readyListener.listen(2003);
	killListener.listen(2004);
	p2LevelListener.listen(2005);
	playerListener.setBlocking(false);
	killThread = false;
	waitScreen = true;
	waitTimer = true;
	time(&currentTime);//intializes time
	time(&endTime);
	endTime += 45;//sets time until thread is killed
	acThreads[0] = std::thread(&Network::networkObjects::hostConnect, this, ref(waitScreen), ref(killListener), ref(playerListener), ref(scoreListener), ref(mapListener), ref(readyListener), ref(p2LevelListener), ref(connectionType), ref(msMessage), ref(waitTimer));
	acThreads[0].detach();
	waitMessage = "\tWaiting For Client Connection\t\nApproximate Time Remaining: ";
}

void Network::networkObjects::findLocalHost(bool& waitScreenT, TcpSocket& killSocketT, TcpSocket& playerSocketT, TcpSocket& scoreSocketT, 
	TcpSocket& mapSocketT, TcpSocket& readySocketT, TcpSocket& p2LevelSocketT,
	char& connectionTypeT, int(&msMessageT)[4], bool& waitTimerT)//attemps to find a computer hosting a server and connect to it
{
	std::string ipS = userIP.toString();

	while (ipS.at(ipS.size() - 1) != '.')//pops back of string until a . is reached
		ipS.pop_back();

	for (int i = 0; i < 1000 && !killThread; i++) {//tries to find a ip by testing new endings
		ostringstream ipSS;

		ipSS << ipS << i;

		userIP = ipSS.str();
		//attempts to join sever
		sf::Socket::Status status = playerSocketT.connect(userIP, 2000, sf::milliseconds(20));
		if (status == sf::Socket::Done) {
			status = scoreSocketT.connect(userIP, 2001, sf::milliseconds(100));
			if (status == sf::Socket::Done) {
				status = mapSocketT.connect(userIP, 2002, sf::milliseconds(100));
				if (status == sf::Socket::Done) {
					status = readySocketT.connect(userIP, 2003, sf::milliseconds(100));
					if (status == sf::Socket::Done) {
						status = killSocketT.connect(userIP, 2004, sf::milliseconds(100));
						if (status == sf::Socket::Done) {
							status = p2LevelSocketT.connect(userIP, 2005, sf::milliseconds(100));
							if (status == sf::Socket::Done) {//once all sockets have connected
								connectionTypeT = 'c';
								msMessageT[0] = 600;
								killSocket.setBlocking(false);
								playerSocket.setBlocking(false);
								scoreSocket.setBlocking(false);
								mapSocket.setBlocking(false);
								readySocket.setBlocking(false);
								p2LevelSocket.setBlocking(false);
								break;
							}
						}
					}
				}
			}
		}
		time(&currentTime);
	}
	if (connectionTypeT == ' ')
		msMessageT[1] = 600;

	waitScreenT = false;
	waitTimerT = false;
}
void Network::networkObjects::clientConnect(bool& waitScreenT, TcpSocket& killSocketT, TcpSocket& playerSocketT, TcpSocket& scoreSocketT, TcpSocket& mapSocketT, TcpSocket& readySocketT, 
	TcpSocket& p2LevelSocketT, char& connectionTypeT, int(&msMessageT)[4], bool& waitTimerT)//attempts to join the ip entered by the user
{
	while (endTime - currentTime > 0 && !killThread) {
		sf::Socket::Status status = playerSocketT.connect(userIP, 2000, sf::milliseconds(100));
		if (status == sf::Socket::Done) {
			status = scoreSocketT.connect(userIP, 2001, sf::milliseconds(100));
			if (status == sf::Socket::Done) {
				status = mapSocketT.connect(userIP, 2002, sf::milliseconds(100));
				if (status == sf::Socket::Done) {
					status = readySocketT.connect(userIP, 2003, sf::milliseconds(100));
					if (status == sf::Socket::Done) {
						status = killSocketT.connect(userIP, 2004, sf::milliseconds(100));
						if (status == sf::Socket::Done) {
							status = p2LevelSocketT.connect(userIP, 2005, sf::milliseconds(100));
							if (status == sf::Socket::Done) {//once all sockets have connected
								connectionTypeT = 'c';
								msMessageT[0] = 600;
								killSocket.setBlocking(false);
								playerSocket.setBlocking(false);
								scoreSocket.setBlocking(false);
								mapSocket.setBlocking(false);
								readySocket.setBlocking(false);
								p2LevelSocket.setBlocking(false);
								break;
							}
						}
					}
				}
			}
		}

		time(&currentTime);
	}
	//  Can ask user for Username, will be used to display score on scoreboard

	if (connectionTypeT != 'c')
		msMessageT[1] = 600;

	waitScreenT = false;
	waitTimerT = false;
}

void Network::networkObjects::hostConnect(bool& waitScreenT, TcpListener& killListenerT, TcpListener& playerListenerT, TcpListener& scoreListenerT, TcpListener& mapListenerT, TcpListener& readyListenerT,
	TcpListener& p2LevelListenerT, char& connectionTypeT, int(&msMessageT)[4], bool& waitTimerT)
{
	while (endTime - currentTime > 0 && !killThread) {
		time(&currentTime);

		sf::Socket::Status status = playerListenerT.accept(playerSocket);
		if (status == sf::Socket::Done) {	//  Can ask user for Username, will be used to display score on scoreboard
			status = scoreListenerT.accept(scoreSocket);
			if (status == sf::Socket::Done) {
				status = mapListenerT.accept(mapSocket);
				if (status == sf::Socket::Done) {
					status = readyListenerT.accept(readySocket);
					if (status == sf::Socket::Done) {
						status = killListenerT.accept(killSocket);
						if (status == sf::Socket::Done) {
							status = p2LevelListenerT.accept(p2LevelSocket);
							if (status == sf::Socket::Done) {//once all sockets have been accepted
								connectionTypeT = 's';
								msMessageT[0] = 600;
								playerListener.setBlocking(true);
								playerSocket.setBlocking(false);
								killSocket.setBlocking(false);
								scoreSocket.setBlocking(false);
								mapSocket.setBlocking(false);
								readySocket.setBlocking(false);
								p2LevelSocket.setBlocking(false);
								break;
							}
						}
					}
				}
			}
		}
	}
	if (connectionTypeT != 's') {// if it fails, close the listeners
		msMessageT[1] = 600;

		playerListener.close();
		scoreListener.close();
		mapListener.close();
		readyListener.close();
		killListener.close();
		p2LevelListener.close();
	}
	waitScreenT = false;
	waitTimerT = false;
}

void Network::networkObjects::killThreads()//kills running threads
{
	killThread = true;
	waitScreen = false;
}

void Network::networkObjects::startMultiplayer()//starts multiplayer
{
	msMessage[2] = 600;
	multiplayerGame = true;
	clientUsername = "Client";
}