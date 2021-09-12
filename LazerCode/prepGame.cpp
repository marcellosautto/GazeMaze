#include "pch.h"
#include "prepGame.h"

prepGame::prepGame(Network& networkT)//constructor
{
	network = &networkT;
	mazeS.resize(1); //resizes sprite vector
	mazeS.at(0).resize(0);

	mazeV.resize(1); //resizes maze vector
	mazeV.at(0).resize(0);

	//load sfml font
	if (!Calibri.loadFromFile("Assets/Font/Calibri 400.ttf"))
		errorMessageExit("Text Load Fail");

	if (!Ponde.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");

	if (!Atarian.loadFromFile("Assets/Font/SF Atarian System.ttf"))
		errorMessageExit("Failed to load text.");

	//Set sfml text
	timeTxt.setFont(Atarian);
	timeTxt.setCharacterSize(100);
	timeTxt.setFillColor(Color::Red);
	timeTxt.setPosition(1200, 50);

	levelTxt.setFont(Atarian);
	levelTxt.setCharacterSize(100);
	levelTxt.setPosition(1200, 175);

	videoTxt.setFont(Ponde);
	videoTxt.setString("MOTION CAPTURE DISPLAY");
	videoTxt.setStyle(sf::Text::Style::Underlined);
	videoTxt.setCharacterSize(36);
	videoTxt.setPosition(1152, 350);

	rightTxt.setFont(Ponde);
	leftTxt.setFont(Ponde);
	upTxt.setFont(Ponde);
	downTxt.setFont(Ponde);
	oTxt.setFont(Ponde);

	rightTxt.setString("RIGHT");
	leftTxt.setString("LEFT");
	upTxt.setString("UP");
	downTxt.setString("DOWN");
	oTxt.setString("ORIGIN");

	rightTxt.setCharacterSize(50);
	leftTxt.setCharacterSize(50);
	upTxt.setCharacterSize(50);
	downTxt.setCharacterSize(50);
	oTxt.setCharacterSize(50);

	rightTxt.setPosition(1375, 950);
	leftTxt.setPosition(1375, 950);
	upTxt.setPosition(1375, 950);
	downTxt.setPosition(1375, 950);
	oTxt.setPosition(1375, 950);

	for (int i = 0; i < hedgeSpriteCount; i++) //loads and creates hedge sprites 
		hedgeSpriteManager.CreateSprite(to_string(i), "Assets/MazeArt/Hedges/Hedge" + to_string(i) + ".png");
	for (int i = 6; i < hedgeSpriteCount; i++)
		hedgeSpriteManager.CreateSprite(to_string(i * 10 + 1), "Assets/MazeArt/Hedges/Hedge" + to_string(i) + "1.png");
	hedgeSpriteManager.CreateSprite(to_string(62), "Assets/MazeArt/Hedges/Hedge62.png");
	hedgeSpriteManager.CreateSprite(to_string(72), "Assets/MazeArt/Hedges/Hedge72.png");
	hedgeSpriteManager.CreateSprite("path", "Assets/MazeArt/Path/Path.png");
	hedgeSpriteManager.CreateSprite("line1", "Assets/MazeArt/Path/Line1.png");
	hedgeSpriteManager.CreateSprite("line2", "Assets/MazeArt/Path/Line2.png");

	//Loads Sprites 
	if (!backgroundT.loadFromFile("Assets/Menus/menuBack.png"))
		errorMessageExit("Texture Load Fail");
	backgroundS.setTexture(backgroundT);

	if (network->nO.multiplayerGame && network->gO.currentLevel() == 0) //Sends map from host to client / inverse 
		setMultiplayer();
	else if (!network->nO.multiplayerGame)
		generateMaze(); //generates maze

	if (network->nO.multiplayerGame)//intializes mazeV
	{
		mazeV.clear();
		for (int i = 0; i < network->gO.mazeTransferVector.at(0).size(); i++) {
			mazeV.resize(mazeV.size() + 1);

			for (int j = 0; j < network->gO.mazeTransferVector.at(0).at(i).size(); j++)
				mazeV.at(i).emplace_back(network->gO.mazeTransferVector[network->gO.currentLevel()][i][j]);
		}

		outputTxt();
	}
	buildMaze();//builds the sprite maze
}

void prepGame::setMultiplayer()
{
	int M = 2 * m + 1;
	int N = 2 * n + 1;
	int a, b, c;
	a = network->gO.totalLevels;
	b = M;
	c = N;
	int k = 0;
	mazeV.resize(M, vector<char>(N));
	mazeV.at(0).resize(N);

	Packet tempPacket;//waits for both players before continuing
	network->nO.mapSocket.setBlocking(true);
	tempPacket << true;
	network->nO.mapSocket.send(tempPacket);
	network->nO.mapSocket.receive(tempPacket);
	network->nO.mapSocket.setBlocking(false);
	char buffer[2000] = { 0 };
	size_t received;

	// The server will generate map(s) and then send it to the client
	if (network->nO.connectionType == 's')
	{
		network->gO.mazeTransferVector.resize(a, vector< vector < char > >(b, vector<char>(c)));
		string text = to_string(network->gO.totalLevels);
		network->nO.mapSocket.send(text.c_str(), text.length() + 1);
		for (int levelCount = 0; levelCount < network->gO.totalLevels; levelCount++)
		{
			string text = "";
			generateMaze(); //creates random maze, saves it to txt file Assets/mazeTxt.txt
			for (int i = 0; i < mazeV.size(); i++)
			{
				for (int j = 0; j < mazeV.at(i).size(); j++)
				{
					text += mazeV.at(i).at(j);
					network->gO.mazeTransferVector[levelCount][i][j] = mazeV.at(i).at(j);

				}
			}
			network->nO.mapSocket.send(text.c_str(), text.length() + 1);
		}
	}
	// The client will receive the map and use it for their maze game
	if (network->nO.connectionType == 'c')
	{
		network->nO.mapSocket.setBlocking(true);
		network->nO.mapSocket.receive(buffer, sizeof(buffer), received);
		network->gO.totalLevels = int(buffer[0]) - 48;
		a = network->gO.totalLevels;
		network->gO.mazeTransferVector.resize(a, vector< vector < char > >(b, vector<char>(c)));
		for (int levelCount = 0; levelCount < network->gO.totalLevels; levelCount++)
		{
			network->nO.mapSocket.setBlocking(true);
			network->nO.mapSocket.receive(buffer, sizeof(buffer), received);
			k = 0;
			for (int i = 0; i < mazeV.size(); i++)
			{
				for (int j = 0; j < mazeV.at(i).size(); j++)
				{
					network->gO.mazeTransferVector[levelCount][i][j] = buffer[k];
					k++;
				}
			}
		}
	}
	network->gO.levelsRemaining = network->gO.totalLevels;
}

void prepGame::buildMaze()
{
	for (int y = 0; y < mazeV.size(); y++) {

		mazeS.resize(mazeS.size() + 1);//Dynamically resizes outer vector 

		for (int x = 0; x < mazeV.at(y).size(); x++) {//assigns sprites to inner vector

			if (mazeV[y][x] == '1')//If equal to 1, hedge, find the surroundings to pick the correct sprite
				mazeS.at(y).emplace_back(hedgeSpriteManager.GetSprite(to_string(findSurrounding(y, x))));

			else if (mazeV[y][x] == '0') {//if 0 assign path, or gate, depending on position

				if (y == 0)
					mazeS.at(y).emplace_back(hedgeSpriteManager.GetSprite("line1"));

				else if (y == (mazeV.at(y).size() - 1))
					mazeS.at(y).emplace_back(hedgeSpriteManager.GetSprite("line2"));

				else
					mazeS.at(y).emplace_back(hedgeSpriteManager.GetSprite("path"));
			}
		}
	}
}

void prepGame::inputTxt()
{
	ifstream inF("Assets/mazeTxt.txt");
	char inC = ' ';

	while (!inF.eof()) {//while there are still move values of inF, repeat loop

		inC = inF.get();//assign current value to inC

		if (inC == '\n') //if inC is next line, create a new outer vector
			mazeV.resize(mazeV.size() + 1);

		else//If inC is a 0 or 1 place it to the top of the current vector 
			mazeV.at(mazeV.size() - 1).emplace_back(inC);
	}

	inF.close();//close inF
}

void prepGame::outputTxt()//outputs maze to a text file
{
	ofstream mazeTxt;
	mazeTxt.open("Assets/mazeTxt.txt");

	for (int i = 0; i < mazeV.size(); i++) {
		for (int j = 0; j < mazeV.at(i).size(); j++)
			mazeTxt << mazeV.at(i).at(j);

		mazeTxt << '\n';
	}
	mazeTxt.close();
}

int prepGame::findSurrounding(int y, int x)//finds surroundings of a sprite to pick a sprite for the maze
{
	int checker = 0;

	if (x - 0 > 0 && (mazeV[y][x - 1] == '1' || (y == 0 && x == 2) || (y == 18 && x == 18)))//If there is a value to the left  of x,y add 1 to checker
		checker += 1;

	if (y < mazeV.size() - 1 && mazeV[y + 1][x] == '1')//If there is a value above x,y add 10 to checker
		checker += 10;

	if (x < mazeV.at(y).size() - 1 && (mazeV[y][x + 1] == '1' || (y == 0 && x == 0) || (y == 18 && x == 16)))//If there is a value to the right of x,y add 100 to checker
		checker += 100;

	if (y - 0 > 0 && mazeV[y - 1][x] == '1')//If there is a value below x,y add 1000 to checker
		checker += 1000;

	if (checker == 0)//If statement picks correct sprite and returns it number depending on number
		return 1;

	else if (checker == 1)
		return 2;

	else if (checker == 1000)
		return 3;

	else if (checker == 100)
		return 4;

	else if (checker == 10)
		return 5;

	else if (checker == 101)
		if (y == 0)
			return 61;
		else if (y == 18)
			return 62;
		else
			return 6;

	else if (checker == 1010)
		if (x == 0)
			return 72;
		else if (x == 18)
			return 71;
		else
			return 7;

	else if (checker == 11)
		if (y == 0)
			return 81;
		else
			return 8;

	else if (checker == 1001)
		if (y == 18)
			return 91;
		else
			return 9;

	else if (checker == 1100)
		if (y == 18)
			return 101;
		else
			return 10;

	else if (checker == 110)
		if (y == 0)
			return 111;
		else
			return 11;

	else if (checker == 1101)
		if (y == 18)
			return 121;
		else
			return 12;

	else if (checker == 1110)
		if (x == 0)
			return 131;
		else
			return 13;

	else if (checker == 111)
		if (y == 0)
			return 141;
		else
			return 14;

	else if (checker == 1011)
		if (x == 18)
			return 151;
		else
			return 15;

	else if (checker == 1111)
		return 0;
}


/********************************************************
 *
 * Author: Akshat Malviya (@akshat157)
 * Date: Sunday, June 07
 * Desc: A random maze generator using backtracking!
 *
*********************************************************/

// A utility function to get the index of cell with indices x, y;
int prepGame::getIdx(int x, int y, vector < pair<int, pair<int, int> > > cell_list) {
	for (int i = 0; i < cell_list.size(); i++)
	{
		if (cell_list[i].second.first == x && cell_list[i].second.second == y)
			return cell_list[i].first;
	}
	return -1;
}

void prepGame::createMaze(int M, int N) {

	vector < pair<int, pair<int, int> > > cell_list;
	vector <bool> visited(m * n, false);
	stack<pair<int, pair<int, int> > > m_stack;
	random_device rdev;
	mt19937 rng(rdev());
	uniform_int_distribution<mt19937::result_type> dist100(1, 100);

	int nVisited = 0;
	int k = 0;

	for (int i = 1; i < M; i += 2) {
		for (int j = 1; j < N; j += 2) {
			cell_list.push_back(make_pair(k, make_pair(i, j)));
			k++;
		}
	}

	int randIdx = dist100(rng) % m * n;
	m_stack.push(cell_list[randIdx]);
	visited[randIdx] = true;
	nVisited++;

	// Algo
	while (nVisited < m * n) {

		vector <int> neighbours;
		// North
		if (m_stack.top().second.first > 1) {
			if (mazeV[m_stack.top().second.first - 2][m_stack.top().second.second + 0] &&
				!visited[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]) {
				neighbours.push_back(0);
			}
		}
		// East
		if (m_stack.top().second.second < N - 2) {
			if (mazeV[m_stack.top().second.first + 0][m_stack.top().second.second + 2] &&
				!visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]) {
				neighbours.push_back(1);
			}
		}
		// South
		if (m_stack.top().second.first < M - 2) {
			if (mazeV[m_stack.top().second.first + 2][m_stack.top().second.second + 0] &&
				!visited[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]) {
				neighbours.push_back(2);
			}
		}
		// West
		if (m_stack.top().second.second > 1) {
			if (mazeV[m_stack.top().second.first + 0][m_stack.top().second.second - 2] &&
				!visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)]) {
				neighbours.push_back(3);
			}
		}
		// Neighbours available?
		if (!neighbours.empty()) {
			// Choose a random direction
			int next_cell_dir = neighbours[dist100(rng) % neighbours.size()];
			// Create a path between this cell and neighbour
			switch (next_cell_dir) {
			case 0: // North
				mazeV[m_stack.top().second.first - 1][m_stack.top().second.second + 0] = '0';
				m_stack.push(cell_list[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]);
				break;
			case 1: // East
				mazeV[m_stack.top().second.first + 0][m_stack.top().second.second + 1] = '0';
				m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]);
				break;
			case 2: // South
				mazeV[m_stack.top().second.first + 1][m_stack.top().second.second + 0] = '0';
				m_stack.push(cell_list[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]);
				break;
			case 3: // West
				mazeV[m_stack.top().second.first + 0][m_stack.top().second.second - 1] = '0';
				m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)]);
				break;
			}

			visited[m_stack.top().first] = true;
			nVisited++;
		}
		else {
			m_stack.pop();
		}
	}
}

void prepGame::generateMaze() {

	int M = 2 * m + 1;
	int N = 2 * n + 1;
	mazeV.resize(M, vector<char>(N));
	mazeV.at(0).resize(N);

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (!(i & 1) || !(j & 1))
				mazeV[i][j] = '1';
			else
				mazeV[i][j] = '0';

		}
	}

	for (int i = 1; i < M; i += 2) {
		for (int j = 1; j < N; j += 2) {
			mazeV[i][j] = '0';
		}

	}
	createMaze(M, N);
	mazeV[0][1] = '0';
	mazeV[2 * m][2 * n - 1] = '0';
	outputTxt();
}