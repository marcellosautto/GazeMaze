#include "pch.h"
//#include "driver.h"
using namespace sf;

class ScoreScreen
{
public:
	bool ScoreWindow();
	ScoreScreen();
	~ScoreScreen();
	void DrawBackground(RenderTarget &RenderTarget);
	int* ReadFile(int &count);

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
};
