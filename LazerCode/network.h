#pragma once
#include "pch.h"

using namespace sf;

class Network {

public:

	struct gameObjects {//network struct for game objects
		bool multiplayerWait; //multiplayer/single player
		vector<vector<vector<char>>> mazeTransferVector;//contains a saved version of all mazes 
		int currentLevel(), //returns current level
			totalLevels, levelsRemaining, Player2Time;
		Vector2f p1PrevPosition, p2PrevPosition;

		gameObjects();//constructor
	}gO;

	struct networkObjects {//network struct for network objects
	public:
		TcpSocket playerSocket, scoreSocket, mapSocket, readySocket, killSocket, p2LevelSocket;// sockets to send data
		IpAddress userIP;//ip address 
		string hostUsername, clientUsername, waitMessage;
		Packet tempPacket, readyPacket; // packets of information to send through sockets
		TcpListener playerListener, scoreListener, mapListener, readyListener, killListener, p2LevelListener; //connects to other player through port
		char buffer[2000] = { 0 }, connectionType;
		bool waitTimer, waitScreen, multiplayerGame, killThread;
		time_t endTime, currentTime;//timers for connecting to other player
		int msMessage[4] = { 0 };
		std::thread acThreads[3];//array of threads

		void prepHostConnect(),//code run before host connect is launched on its own thread
			prepclientConnect(string userIPT),//code run before client connect is launched on its own thread
			prepLocalHostConnect(),// code run before local host connect is launched on its own thread
			startMultiplayer(),//starts multiplayer
			killThreads(),//kills running threads
			sendDisconnectSignal(),//signals to other player, that the player has disconnected
			sendUsernames();//sends username to other player

	
		void hostConnect(bool& waitScreenT, TcpListener& killListenerT, TcpListener& playerListenerT, TcpListener& scoreListenerT, TcpListener& mapListenerT, TcpListener& readyListenerT,
			TcpListener& p2LevelListenerT, char& connectionTypeT, int(&msMessageT)[4], bool& waitTimerT),//hosts a server, waits for client connection
			clientConnect(bool& waitScreenT, TcpSocket& killSocketT, TcpSocket& playerSocketT, TcpSocket& scoreSocketT, TcpSocket& mapSocketT, TcpSocket& readySocketT,
				TcpSocket& p2LevelSocketT, char& connectionTypeT, int(&msMessageT)[4], bool& waitTimerT),//attempts to join the ip entered by the user
			findLocalHost(bool& waitScreenT, TcpSocket& killSocketT, TcpSocket& playerSocketT, TcpSocket& scoreSocketT, TcpSocket& mapSocketT, TcpSocket& readySocketT,
				TcpSocket& p2LevelSocketT, char& connectionTypeT, int(&msMessageT)[4], bool& waitTimerT);//attemps to find a computer hosting a server and connect to it

			networkObjects();
			~networkObjects();
	}nO;

};