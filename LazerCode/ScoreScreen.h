#include "pch.h"
#include "Button.h"
#include "network.h"
using namespace sf;

class ScoreScreen
{
public:
	int ScoreWindow(RenderWindow& window, int levelCount, int& menu, bool &advance);
	ScoreScreen(Network& networkT);
	vector<vector<double>> ReadFile(int& count);
	void setMultiplayer(bool temp);
	void setServerTime(const double temp);
	void setClientTime(const double temp);
	void setLevel(const int temp);
	double getServerTime() { return ServerTime; }
	double getClientTime() { return ClientTime; }
	void UpdateScoreScreen();
	void recordScore(double MazeTime);
	void setRecentLevel(int temp);
	Sound trumpSound;

private:
	Event windowEvent;
	Texture Background, HighScoreMenuScreen;
	int RecentLevel;

	Sprite SpriteBackground;
	Font font;

	SoundBuffer trumpBuf;

	std::vector<UIButton*> buttons;

	Text MainMenu, MazeGame, Quit, HighScores, PlayerScores, levelCountT, levelT;
	bool PlayMultiplayer;
	//char connectionType;
	double ServerTime, ClientTime;
	int Level, lastLevelPlayed;
	bool ModifyScores;
	string StringText;

	Network* network = NULL;
};