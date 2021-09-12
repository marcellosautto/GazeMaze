#include "pch.h"
#include "ScoreScreen.h"

// Defaut constructor initializes values for scorescreen
ScoreScreen::ScoreScreen(Network& networkT)
{
	PlayMultiplayer = false;
	ServerTime = ClientTime = Level = RecentLevel = lastLevelPlayed = 0;
	ModifyScores = true;
	network = &networkT;

	if (!Background.loadFromFile("Assets/Menus/menuBack.png"))
		errorMessageExit("Failed to load texture.");
	SpriteBackground.setTexture(Background);

	if (!font.loadFromFile("Assets/Font/SF Atarian System.ttf"))
		errorMessageExit("Failed to load text.");

	if (!trumpBuf.loadFromFile("Assets/High Score Assets/trumpets.wav"))
		errorMessageExit("Failed to load sound.");
	trumpSound.setBuffer(trumpBuf);

	buttons.push_back(new UIButton("Assets/Menus/BackToMenu.png", 150, 850, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/PlayGame.png", 700, 850, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/Exit.png", 1250, 850, 0.5f));
	buttons.push_back(new UIButton("Assets/Menus/plus.png", 1750, 500, 0.15f));
	buttons.push_back(new UIButton("Assets/Menus/minus.png", 1500, 500, 0.15f));

	// High Scores
	HighScores.setFont(font);
	HighScores.setFillColor(Color::Red);
	HighScores.setCharacterSize(120);
	HighScores.setStyle(Text::Bold | Text::Underlined);
	HighScores.setPosition(200, 100);

	// Player scores
	PlayerScores.setFont(font);
	PlayerScores.setFillColor(Color::White);
	PlayerScores.setCharacterSize(80);
	PlayerScores.setPosition(200, 150);

	// Level count
	levelCountT.setFont(font);
	levelCountT.setCharacterSize(100);
	levelCountT.setPosition(1650, 500);
	levelCountT.setFillColor(sf::Color::White);

	levelT.setFont(font);
	levelT.setCharacterSize(100);
	levelT.setPosition(1500, 375);
	levelT.setString("Level Count");
}

// Records actions when user is in the scorescreen window
// Displays attributes of the scorescreen
int ScoreScreen::ScoreWindow(RenderWindow& window, int levelCount, int& menu, bool& advance)
{
	//DrawBackground(window);
	window.draw(SpriteBackground);
	window.draw(HighScores);
	window.draw(PlayerScores);

	// get mouse position
	auto mouse_pos = Mouse::getPosition(window); // Mouse position relative to the Window
	auto translated_pos = window.mapPixelToCoords(mouse_pos); // Mouse position to translated into world coordinates

	if (buttons[0]->IsPressed())// go to main menu screen
		menu = 0;

	else if (buttons[1]->IsPressed())// go to maze game screen
		menu = 5;

	else if (buttons[2]->IsPressed())// go quit program
		menu = 3;

	else if (buttons[3]->IsPressed() && levelCount < 9) {
		levelCount++;
		setLevel(levelCount);
		ModifyScores = false;
		UpdateScoreScreen();
	}

	else if (buttons[4]->IsPressed() && levelCount > 1) {
		levelCount--;
		setLevel(levelCount);
		ModifyScores = false;
		UpdateScoreScreen();
	}

	for (short i = 0; i < buttons.size(); i++)
		buttons[i]->DrawButton(window);

	levelCountT.setString(to_string(levelCount));
	window.draw(levelCountT);
	window.draw(levelT);
	advance = true;

	return levelCount;
}

// Mutator function to set the most recent level
// Used when displaying scores of multiple levels, 
// and remembering what level the user played last
void ScoreScreen::setRecentLevel(int temp)
{
	RecentLevel = temp;
}

// Function updates the values that are shown in the scorescreen
void ScoreScreen::UpdateScoreScreen()
{
	int count = 0;
	bool SetHighScore = false;
	string StringText;
	double ServerTimeRounded, iRounded;
	ServerTimeRounded = std::ceil(ServerTime * 100.0) / 100.0;

	if (!PlayMultiplayer)	// Single player
	{
		vector<vector<double>> Scores = ReadFile(count);
		count = 1;
		for (double i : Scores.at(Level - 1))
		{
			StringText += "\n" + to_string(count) + ") " + to_string(i) + " seconds";
			iRounded = std::ceil(i * 100.0) / 100.0;
			// RecentLevel is what level the user played in the last game
			// Level is changed when user presses '+' or '-'
			// only show recent run if user is in their recently played level
			// and the score matches their recent run
			if (ServerTimeRounded == iRounded && RecentLevel == Level)
			{
				StringText += " (Recent run)";
				SetHighScore = true;
			}
			count++;
		}
		// If the user finished the game but did not get a high score
		// Their score is shown as not a high score
		if (!SetHighScore && ServerTime != 0 && RecentLevel == Level)
		{
			StringText += "\n" + to_string(ServerTime) + " seconds (Recent run, not a high score)";
		}
	}
	else // Multiplayer
	{
		// Both players finished the game, and the server finished first
		if (ServerTime < ClientTime && ServerTime != 0)
		{
			StringText = "\n" + to_string(1) + ") " + network->nO.hostUsername + " " + to_string(ServerTime) + " seconds";
			StringText += "\n" + to_string(2) + ") " + network->nO.clientUsername + " " + to_string(ClientTime) + " seconds";
		}
		// Both players finished the game, and the client finished first
		else if (ClientTime < ServerTime && ClientTime != 0)
		{
			StringText = "\n" + to_string(1) + ") " + network->nO.clientUsername + " " + to_string(ClientTime) + " seconds";
			StringText += "\n" + to_string(2) + ") " + network->nO.hostUsername + " " + to_string(ServerTime) + " seconds";
		}
		// Server finished but waiting for the client
		else if (ServerTime != 0)
		{
			StringText = "\n" + to_string(1) + ") " + network->nO.hostUsername + " " + to_string(ServerTime) + " seconds";
			StringText += "\n" + to_string(2) + ") " + network->nO.clientUsername + "  - still playing...";
		}
		// Client finished but waiting for the host
		else if (ClientTime != 0)
		{
			StringText = "\n" + to_string(1) + ") " + network->nO.clientUsername + " " + to_string(ClientTime) + " seconds";
			StringText += "\n" + to_string(2) + ") " + network->nO.hostUsername + "  - still playing...";
		}
	}
	ModifyScores = true;
	HighScores.setString("High Scores - Level " + to_string(Level));
	PlayerScores.setString(StringText);

}

// Function is used in multiplayer to record the other players score
void ScoreScreen::recordScore(double MazeTime)
{

	char Tbuffer[2000];
	size_t Treceived;
	string text = to_string(MazeTime);
	network->nO.scoreSocket.send(text.c_str(), text.length() + 1);			// send score to player 2
	network->nO.scoreSocket.setBlocking(true);								// block socket until player 2's score is received
	network->nO.scoreSocket.receive(Tbuffer, sizeof(Tbuffer), Treceived);	// receive player 2's score
	string::size_type sz;   // alias of size_t
	double Player2Time = stod(Tbuffer, &sz);								// convert player 2's score from string to double
	if (network->nO.connectionType == 's')									// If host, store client's score
	{
		setClientTime(Player2Time);
	}
	else if (network->nO.connectionType == 'c')								// If client, store host's score
	{
		setServerTime(Player2Time);
	}
}

// Mutator function to set multiplayer on or off
void ScoreScreen::setMultiplayer(bool temp)
{
	PlayMultiplayer = temp;
}

// Mutator function to set Server (host) time to complete maze game
void ScoreScreen::setServerTime(const double temp)
{
	ServerTime = temp;
}

// Mutator function to set client's time to complete maze game
void ScoreScreen::setClientTime(const double temp)
{
	ClientTime = temp;
}

// Mutator function to set the Level of the maze game
void ScoreScreen::setLevel(const int temp)
{
	Level = temp;
}

// Only used in single player
// Function is used to read a file and output to a file
// The data in the file are the 5 best scores for the first 9 levels
// Pre: User selects to view scores
// Post: The scores in the scorescreen are updated depending if user achieved a 
// high score from playing maze game
vector<vector<double>> ScoreScreen::ReadFile(int& count)
{
	Level--;
	int row = 10;
	int col = 5;
	double value = 0;
	double foo = -1;
	string temp;
	fstream InFile;
	ofstream OutFile;
	string line;
	int rowCounter = 0;
	int colCounter = 0;
	bool AddedValue = false;
	vector<vector<double>> Scores(row);								//2D int vector that keeps track of 
																	// 5 best scores for each level(10 in total)

	InFile.open("Assets/High Score Assets/HighScore.txt", ios::in);
	if (!InFile)
		errorMessageExit("Failed to load High Score results.");

	if (InFile.is_open())
	{
		while (getline(InFile, line))
		{
			for (int i = 0; i < line.size() && colCounter < 5; i++)
			{
				if (line[i] != ' ')
				{
					temp += line[i];
				}
				if (line[i + 1] == ' ' || i + 1 == line.size())
				{
					stringstream helper(temp);
					helper >> value;
					if (foo != value)
					{
						Scores.at(rowCounter).push_back(value);
						colCounter++;
					}
					temp = "";
					foo = value;
				}
			}
			rowCounter++;
			colCounter = 0;
		}
		InFile.close();
	}
	rowCounter = 0;
	colCounter = 0;
	if (ServerTime != 0 && ModifyScores)
	{
		OutFile.open("Assets/High Score Assets/HighScore.txt");
		if (!OutFile)
			errorMessageExit("Failed to update High Score results.");

		if (Scores.at(Level).size() == 0)
		{
			Scores.at(Level).push_back(ServerTime);
			AddedValue = true;
		}
		else
		{
			int Element = 0;
			for (double i : Scores.at(Level))
			{
				if (ServerTime < i && !AddedValue)
				{
					Scores.at(Level).insert(Scores.at(Level).begin() + Element, ServerTime);
					AddedValue = true;
					Element++;
				}
			}
			if (Scores.at(Level).size() < 5 && !AddedValue)
			{
				Scores.at(Level).push_back(ServerTime);
				AddedValue = true;
			}

		}
		if (Scores.at(Level).size() > 5)
			Scores.at(Level).pop_back();

		sort(Scores.at(Level).begin(), Scores.at(Level).end());
		rowCounter = 0;
		colCounter = 0;
		while (rowCounter < row)
		{
			for (double i : Scores.at(rowCounter))
			{
				if (colCounter != Scores.at(rowCounter).size())
					OutFile << i << " ";
				else
					OutFile << i;
				colCounter++;
			}
			colCounter = 0;
			OutFile << endl;
			rowCounter++;
		}
		OutFile.close();
	}
	Level++;
	return Scores;
}