#include "pch.h"
using namespace sf;

class ScoreScreen
{
public:
	bool ScoreWindow();
	ScoreScreen();
	~ScoreScreen();
	void DrawBackground(RenderTarget &RenderTarget, RectangleShape rectangle);

private:
	Event windowEvent;
	Texture Background;
	Texture MenuScreen;
	Sprite SBackground;
	Sprite SMenuScreen;
	Font font;

	Text MainMenu;
	Text HighScores;
	Text PlayerScores;
};