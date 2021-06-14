#include "pch.h"
#include "ScoreScreen.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <SFML/Audio.hpp>
#include <chrono>
#include <thread>

using namespace std;

bool ScoreScreen::ScoreWindow()
{
	RenderWindow window(VideoMode(1920, 1080), PROG_NAME);

	RectangleShape rectangle(Vector2f(120, 50));
	rectangle.setSize(Vector2f(150, 75));
	rectangle.setFillColor(Color(119, 136, 153));
	rectangle.setOutlineThickness(10);
	rectangle.setOutlineColor(Color(250, 150, 100));
	rectangle.setPosition(20, 20);

	/*
	if (!MenuScreen.loadFromFile("Assets/High Score Assets/MainMenu.png"))
	{
		errorMessageExit("Failed to load texture.");
	}
	
	SMenuScreen.setTexture(MenuScreen);
	SMenuScreen.setTextureRect(IntRect(100, 100, 200, 200));
	*/

	while (window.isOpen()) 
	{
		DrawBackground(window, rectangle);
		window.display();
		while (window.pollEvent(windowEvent))
		{
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				// get mouse position
				auto mouse_pos = Mouse::getPosition(window); // Mouse position relative to the Window
				auto translated_pos = window.mapPixelToCoords(mouse_pos); // Mouse position to translated into world coordinates
				if (rectangle.getGlobalBounds().contains(translated_pos))
				{
					SoundBuffer buffer;
					if (!buffer.loadFromFile("Assets/High Score Assets/trumpets.wav"))
						return -1;
					Sound sound;
					sound.setBuffer(buffer);
					sound.play();
					cout.flush();
					this_thread::sleep_for(chrono::milliseconds(3000));
					window.close();
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
	int Scores[5];
	int count = 0;
	ifstream InFile;
	string StringText;

	if (!Background.loadFromFile("Assets/BackGround.png"))
	{
		errorMessageExit("Failed to load texture.");
	}
	SBackground.setTexture(Background);
	if (!font.loadFromFile("Assets/Calibri 400.ttf"))
	{
		errorMessageExit("Failed to load text.");
	}

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
	sort(begin(Scores), end(Scores), greater<>());

	for (int i = 0; i < count; i++)
	{
		stringstream ss;
		ss << Scores[i];
		StringText += "\n" + ss.str();
	}

	MainMenu.setFont(font);
	MainMenu.setFillColor(Color::Black);
	MainMenu.setCharacterSize(30);
	MainMenu.setString("Return to \nMain Menu");
	MainMenu.setPosition(20, 20);


	HighScores.setFont(font);
	HighScores.setFillColor(Color::Red);
	HighScores.setCharacterSize(120);
	HighScores.setString("High Scores");
	HighScores.setStyle(Text::Bold | Text::Underlined);
	HighScores.setPosition(200, 100);

	PlayerScores.setFont(font);
	PlayerScores.setFillColor(Color::Blue);
	PlayerScores.setCharacterSize(80);
	PlayerScores.setString(StringText);
	PlayerScores.setPosition(200, 150);
}

ScoreScreen::~ScoreScreen()
{
	
}

// Function prints the GUI that is used to view the high score leaderboard
// Pre: From Previous Screen User selects option to view High Score screen
// Post: User goes back to main menu or quits program
void ScoreScreen::DrawBackground(RenderTarget& RenderTarget, RectangleShape rectangle)
{
	RenderTarget.draw(SBackground);
	RenderTarget.draw(rectangle);
	RenderTarget.draw(MainMenu);
	RenderTarget.draw(HighScores);
	RenderTarget.draw(PlayerScores);
	//RenderTarget.draw(SMenuScreen);
}
