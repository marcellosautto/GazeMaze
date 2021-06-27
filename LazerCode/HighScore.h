#pragma once
#include "pch.h"

// Class that reads and Writes high scores to and from a file
static class HighScore
{
public:
	static bool askForInitials;

	HighScore();

	static bool IsHighScore(int _score);
	static void AddScore(int _score, float _time, char _initials[3]);
	static void Save();

	struct ScoreData {
		ScoreData();
		ScoreData(int _score, float _time, char _initials[3]);

		int score;
		float time;
		char initials[3];
	};

	static std::vector<ScoreData> GetHighScores();

private:
	static ScoreData highScores[10];
};

