#include "pch.h"
#include "ScoreScreen.h"

/*
Notes for improvement:
Create accessor functions for Buttons/texts
*/

int ScoreScreen::ScoreWindow(RenderWindow& window)
{
	DrawBackground(window);
	window.display();

	if (Mouse::isButtonPressed(Mouse::Left))
	{
		// get mouse position
		auto mouse_pos = Mouse::getPosition(window); // Mouse position relative to the Window
		auto translated_pos = window.mapPixelToCoords(mouse_pos); // Mouse position to translated into world coordinates
		if (MainMenuButton.getGlobalBounds().contains(translated_pos))
		{
			SoundBuffer buffer;
			if (!buffer.loadFromFile("Assets/High Score Assets/trumpets.wav"))
				return -1;
			Sound sound;
			sound.setBuffer(buffer);
			sound.play();
			cout.flush();
			this_thread::sleep_for(chrono::milliseconds(3000));
			// go to main menu screen
			window.clear();
			return 0;
		}
		if (MazeGameButton.getGlobalBounds().contains(translated_pos))
		{
			SoundBuffer buffer;
			if (!buffer.loadFromFile("Assets/High Score Assets/trumpets.wav"))
				return -1;
			Sound sound;
			sound.setBuffer(buffer);
			sound.play();
			cout.flush();
			this_thread::sleep_for(chrono::milliseconds(3000));
			// go to maze game screen
			window.clear();
			return 5;
		}
		if (QuitButton.getGlobalBounds().contains(translated_pos))
		{
			SoundBuffer buffer;
			if (!buffer.loadFromFile("Assets/High Score Assets/trumpets.wav"))
				return -1;
			Sound sound;
			sound.setBuffer(buffer);
			sound.play();
			cout.flush();
			this_thread::sleep_for(chrono::milliseconds(3000));
			// go quit program
			window.clear();
			return 3;
		}
	}
	if (windowEvent.type == Event::Closed)
		window.close();
	//window.clear();
	return -1;
}

void ScoreScreen::EndGameTime()
{
	string StringText;
	int count = 0;
	bool SetHighScore = false;
	if (!PlayMultiplayer)
	{
		vector<vector<int>> Scores = ReadFile(count);
		count = 1;
		for (int i : Scores.at(Level))
		{
			int whole = i / 1000;
			int decimal = i % 1000;
			StringText += "\n" + to_string(count) + ") " + to_string(whole) + "." + to_string(decimal) + " seconds";
			if (ServerTime == i)
			{
				StringText += " (Recent run)";
				SetHighScore = true;
			}
			count++;
		}
		if (!SetHighScore && ServerTime != 0)
		{
			int whole = ServerTime / 1000;
			int decimal = ServerTime % 1000;
			StringText += "\n" + to_string(whole) + "." + to_string(decimal) + " seconds (Recent run, not a high score)";
		}
	}
	else
	{
		ServerTime = getServerTime();
		ClientTime = getClientTime();
		int ServerWhole = ServerTime / 1000;
		int ServerDecimal = ServerTime % 1000;
		int ClientWhole = ClientTime / 1000;
		int ClientDecimal = ClientTime % 1000;


		if (ServerTime < ClientTime)
		{
			StringText = "\n" + to_string(1) + ") " + "Server Host: " + to_string(ServerWhole) + "." + to_string(ServerDecimal) + " seconds";
			StringText += "\n" + to_string(2) + ") " + "Client player: " + to_string(ClientWhole) + "." + to_string(ClientDecimal) + " seconds";
		}
		else
		{
			StringText = "\n" + to_string(1) + ") " + "Client player: " + to_string(ClientWhole) + "." + to_string(ClientDecimal) + " seconds";
			StringText += "\n" + to_string(2) + ") " + "Server Host: " + to_string(ServerWhole) + "." + to_string(ServerDecimal) + " seconds";
		}
	}
	// Main Menu rectangle button
	MainMenuButton.setSize(Vector2f(200, 100));
	MainMenuButton.setFillColor(Color(210, 43, 43));
	MainMenuButton.setOutlineThickness(5);
	MainMenuButton.setOutlineColor(Color(0, 0, 0));
	MainMenuButton.setPosition(560, 850);

	// text for main menu button
	MainMenu.setFont(font);
	MainMenu.setFillColor(Color::White);
	MainMenu.setCharacterSize(40);
	MainMenu.setString("Return to \nMain Menu");
	MainMenu.setPosition(580, 850);

	// Play Maze game rectangle button
	MazeGameButton.setSize(Vector2f(200, 100));
	MazeGameButton.setFillColor(Color(210, 43, 43));
	MazeGameButton.setOutlineThickness(5);
	MazeGameButton.setOutlineColor(Color(0, 0, 0));
	MazeGameButton.setPosition(860, 850);

	// text for maze game button
	MazeGame.setFont(font);
	MazeGame.setFillColor(Color::White);
	MazeGame.setCharacterSize(40);
	MazeGame.setString("    Play \nMaze Game");
	MazeGame.setPosition(880, 850);

	// Quit program rectangle button
	QuitButton.setSize(Vector2f(200, 100));
	QuitButton.setFillColor(Color(210, 43, 43));
	QuitButton.setOutlineThickness(5);
	QuitButton.setOutlineColor(Color(0, 0, 0));
	QuitButton.setPosition(1160, 850);

	// text for quit program button
	Quit.setFont(font);
	Quit.setFillColor(Color::White);
	Quit.setCharacterSize(40);
	Quit.setString("    Quit \n  Program");
	Quit.setPosition(1180, 850);

	// High Scores
	HighScores.setFont(font);
	HighScores.setFillColor(Color::Red);
	HighScores.setCharacterSize(120);
	HighScores.setString("High Scores");
	HighScores.setStyle(Text::Bold | Text::Underlined);
	HighScores.setPosition(200, 100);

	// Player scores
	PlayerScores.setFont(font);
	PlayerScores.setFillColor(Color::White);
	PlayerScores.setCharacterSize(80);
	PlayerScores.setString(StringText);
	PlayerScores.setPosition(200, 150);
}

ScoreScreen::ScoreScreen()
{
	PlayMultiplayer = false;
	ServerTime = 0;
	ClientTime = 0;
	if (!Background.loadFromFile("Assets/Menus/menuBack.png"))
	{
		errorMessageExit("Failed to load texture.");
	}
	SpriteBackground.setTexture(Background);
	if (!font.loadFromFile("Assets/Font/SF Atarian System.ttf"))
	{
		errorMessageExit("Failed to load text.");
	}
}

void ScoreScreen::setMultiplayer(bool temp)
{
	PlayMultiplayer = temp;
}

void ScoreScreen::setServerTime(const int temp)
{
	ServerTime = temp;
}

void ScoreScreen::setClientTime(const int temp)
{
	ClientTime = temp;
}

void ScoreScreen::setLevel(const int temp)
{
	Level = temp;
}

vector<vector<int>> ScoreScreen::ReadFile(int& count)
{
	Level--; // Set level equal to its equivalent in the rowCounter of the vector Scores
	int row = 10;
	int col = 5;
	int value = 0;
	int foo = -1;
	string temp;
	fstream InFile;
	ofstream OutFile;
	string line;
	int rowCounter = 0;
	int colCounter = 0;
	bool AddedValue = false;
	vector<vector<int>> Scores(row);								//2D int vector that keeps track of 
																	// 5 best scores for each level(10 in total)

	InFile.open("Assets/High Score Assets/HighScore.txt", ios::in);
	if (!InFile)
	{
		errorMessageExit("Failed to load High Score results.");
	}

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
				if (line[i + 1] == ' ' || i+1 == line.size())
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
		 //To test readability of Scores 
		/*for (int k = 0; k < Scores.size(); k = k + 1) 
		{
			for (int l = 0; l < Scores[k].size(); l = l + 1) 
			{
				cout << Scores[k][l] << ' ';
			}
			cout << endl;
		}*/
		InFile.close();
	}
	rowCounter = 0;
	colCounter = 0;
	if (ServerTime != 0)
	{
		OutFile.open("Assets/High Score Assets/HighScore.txt");
		if (!OutFile)
		{
			errorMessageExit("Failed to update High Score results.");
		}
		if (Scores.at(Level).size() == 0)
		{
			Scores.at(Level).push_back(ServerTime);
			AddedValue = true;
		}
		else
		{
			int Element = 0;
			for (int i : Scores.at(Level))
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
			for (int i : Scores.at(rowCounter))
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
	return Scores;
}

ScoreScreen::~ScoreScreen()
{

}

// Function prints the GUI that is used to view the high score leaderboard
// Pre: From Previous Screen User selects option to view High Score screen
// Post: User goes back to main menu or quits program
void ScoreScreen::DrawBackground(RenderTarget& RenderTarget)
{
	RenderTarget.draw(SpriteBackground);
	RenderTarget.draw(MainMenuButton);
	RenderTarget.draw(MainMenu);
	RenderTarget.draw(MazeGameButton);
	RenderTarget.draw(MazeGame);
	RenderTarget.draw(QuitButton);
	RenderTarget.draw(Quit);
	RenderTarget.draw(HighScores);
	RenderTarget.draw(PlayerScores);
}