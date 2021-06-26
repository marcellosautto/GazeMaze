#include "pch.h"
using namespace sf;

class ScoreScreen
{
public:
	int ScoreWindow(RenderWindow& window);
	ScoreScreen();
	~ScoreScreen();
	void DrawBackground(RenderTarget& RenderTarget);
	vector<vector<int>> ReadFile(int& count);
	void setMultiplayer(bool temp);
	void setServerTime(const int temp);
	void setClientTime(const int temp);
	void setLevel(const int temp);
	int getServerTime() { return ServerTime; }
	int getClientTime() { return ClientTime; }
	void EndGameTime();

private:
	Event windowEvent;
	Texture Background;
	Texture HighScoreMenuScreen;

	Sprite SpriteBackground;
	Font font;

	RectangleShape MainMenuButton;
	RectangleShape MazeGameButton;
	RectangleShape QuitButton;

	Text MainMenu;
	Text MazeGame;
	Text Quit;
	Text HighScores;
	Text PlayerScores;
	bool PlayMultiplayer;
	int ServerTime;
	int ClientTime;
	int Level;
};