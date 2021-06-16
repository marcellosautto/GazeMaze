#include "pch.h"
#include "ScoreScreen.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <SFML/Audio.hpp>
#include <chrono>
#include <thread>
using namespace std;
/*
Notes for improvement:
Create accessor functions for Buttons/texts
*/

bool ScoreScreen::ScoreWindow()
{
	RenderWindow window(VideoMode(1920, 1080), PROG_NAME);
	   
	while (window.isOpen()) 
	{
		DrawBackground(window);
		window.display();
		while (window.pollEvent(windowEvent))
		{
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
					/*menuSwitch mS;
					mS.toMenu();*/
					window.close();
					
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
					window.close();
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
					window.close();
					return false;
				}
			}
			if (windowEvent.type == Event::Closed)
				window.close();
		}
		window.clear();
	}
	return true;
}

ScoreScreen::ScoreScreen()
{
	string StringText;
	int count = 0;
	int*Scores = ReadFile(count);

	if (!Background.loadFromFile("Assets/BackGround.png"))
	{
		errorMessageExit("Failed to load texture.");
	}
	SpriteBackground.setTexture(Background);
	if (!font.loadFromFile("Assets/Calibri 400.ttf"))
	{
		errorMessageExit("Failed to load text.");
	}
	
	for (int i = 0; i < count; i++)
	{
		StringText += "\n" + to_string(i + 1) + ". " + to_string(Scores[i]) + " seconds";
	}

	// Main Menu rectangle button
	MainMenuButton.setSize(Vector2f(150, 75));
	MainMenuButton.setFillColor(Color(173, 216, 230));
	MainMenuButton.setOutlineThickness(5);
	MainMenuButton.setOutlineColor(Color(0, 0, 0));
	MainMenuButton.setPosition(960, 600);

	// text for main menu button
	MainMenu.setFont(font);
	MainMenu.setFillColor(Color::Black);
	MainMenu.setCharacterSize(30);
	MainMenu.setString("Return to \nMain Menu");
	MainMenu.setPosition(960, 600);

	// Play Maze game rectangle button
	MazeGameButton.setSize(Vector2f(150, 75));
	MazeGameButton.setFillColor(Color(173, 216, 230));
	MazeGameButton.setOutlineThickness(5);
	MazeGameButton.setOutlineColor(Color(0, 0, 0));
	MazeGameButton.setPosition(960, 750);

	// text for maze game button
	MazeGame.setFont(font);
	MazeGame.setFillColor(Color::Black);
	MazeGame.setCharacterSize(30);
	MazeGame.setString("Play \nMaze Game");
	MazeGame.setPosition(960, 750);

	// Quit program rectangle button
	QuitButton.setSize(Vector2f(150, 75));
	QuitButton.setFillColor(Color(173, 216, 230));
	QuitButton.setOutlineThickness(5);
	QuitButton.setOutlineColor(Color(0, 0, 0));
	QuitButton.setPosition(960, 900);

	// text for quit program button
	Quit.setFont(font);
	Quit.setFillColor(Color::Black);
	Quit.setCharacterSize(30);
	Quit.setString("Quit \nProgram");
	Quit.setPosition(960, 900);
		
	// High Scores
	HighScores.setFont(font);
	HighScores.setFillColor(Color::Red);
	HighScores.setCharacterSize(120);
	HighScores.setString("High Scores");
	HighScores.setStyle(Text::Bold | Text::Underlined);
	HighScores.setPosition(200, 100);

	// Player scores
	PlayerScores.setFont(font);
	PlayerScores.setFillColor(Color::Blue);
	PlayerScores.setCharacterSize(80);
	PlayerScores.setString(StringText);
	PlayerScores.setPosition(200, 150);
}

int* ScoreScreen::ReadFile(int &count)
{
	static int Scores[5];
	ifstream InFile;

	InFile.open("Assets/High Score Assets/HighScore.txt");
	if (!InFile)
	{
		errorMessageExit("Failed to load High Score results.");
	}

	while (InFile >> Scores[count] && count < 5)
	{
		count++;
	}

	//Close file
	InFile.close();

	// Sort the array in descending order
	sort(begin(Scores), end(Scores), greater<>());\
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


