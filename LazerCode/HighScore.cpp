#include "pch.h"
#include "HighScore.h"

bool HighScore::askForInitials;
HighScore::ScoreData HighScore::highScores[10];

HighScore::HighScore() {
	askForInitials = false;

	// Open the high scores file
	std::ifstream file;
	file.open("Assets/highscores.txt");

	for (int i = 0; i < 10; i++) {
		// Get one line from the file
		std::string line;
		std::getline(file, line);

		ScoreData data;
		std::string value;
		std::stringstream ss(line);

		// read in the initials, score, and time values fom the file
		std::getline(ss, value, ';');
		for (int i = 0; i < 3; i++)
			data.initials[i] = value[i];
		std::getline(ss, value, ';');
		data.score = std::stoi(value);
		std::getline(ss, value, ';');
		data.time = std::stod(value);

		// Store the values in the highScores array
		highScores[i] = data;
	}

	// Close the file
	file.close();
}
std::vector<HighScore::ScoreData> HighScore::GetHighScores() {
	// return the high scores
	std::vector<HighScore::ScoreData> scores;
	for (ScoreData d : highScores)
		scores.push_back(d);
	return scores;
}
bool HighScore::IsHighScore(int _score) {
	// check if a score is a high score or not
	if (_score >= highScores[9].score)
		return true;
	return false;
}
void HighScore::AddScore(int _score, float _time, char _initials[3]) {
	if (IsHighScore(_score)) {
		// Check for the correct position in the array for the new object
		int i;
		for (i = 9; i > 0; i--) {
			if (_score > highScores[i].score) {
				continue;
			}
			break;
		}
		if (_score < highScores[i].score)
			i++;

		// Shift everything else down
		for (int j = 8; j >= i; j--) {
			highScores[j + 1] = highScores[j];
		}

		// insert the new value
		highScores[i] = ScoreData(_score, _time, _initials);
		Save();
		return;
	}
}
void HighScore::Save() {
	// open a file to write to
	std::ofstream file;
	file.open("Assets/highscores.txt");

	// write the values to them
	for (int i = 0; i < 10; i++) {
		for (char c : highScores[i].initials)
			file << c;
		file << ';' << highScores[i].score << ';' << highScores[i].time << ";\n";
	}

	// close the file
	file.close();
}
HighScore::ScoreData::ScoreData() {

}
HighScore::ScoreData::ScoreData(int _score, float _time, char _initials[3]) {
	// constructor for the score data class
	score = _score;
	time = _time;
	for (int i = 0; i < 3; i++) {
		initials[i] = _initials[i];
	}
}