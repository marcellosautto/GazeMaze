#pragma once

#include "pch.h"


class objDet {

public:

	objDet(int argc, const char** argv);

	//driver
	std::array<bool, 5> runMotionDetect(std::array<bool, 5> userInput);

	bool calibrate(sf::RenderWindow& _window);
	bool cameraSwitch(bool useKeyboard);
	void detectMotionHand(cv::Mat frame);
	void setFPS(int cam_device);

	sf::Sprite videoSprite;
	std::vector<cv::Point2d> motionDetectF;
	cv::VideoCapture capture;

	sf::SoundBuffer switchToKeyboardB, switchToMotionB;
	sf::Sound switchToKeyboardS, switchToMotionS;




private:

	int fps;
	int movementsPerSecond;
	char lastDirectionF;
	bool justAttacked;
	cv::Rect originRect;
	cv::Point originPoint;

	sf::Image videoImg;
	sf::Texture videoTexture;

	sf::Image pregameBGIMG;
	sf::Texture pregameBGTexture;
	sf::Sprite pregameBGSprite;


	std::vector<int> scaleInc;

	std::array<bool, 5> movements;
	std::array<bool, 5> calibrationArray;

	sf::Text videoTxt;
	sf::Font font_ponde;

	sf::Text pregame_origin_text;
	sf::Text pregame_up_text;
	sf::Text pregame_down_text;
	sf::Text pregame_left_text;
	sf::Text pregame_right_text;

	sf::SoundBuffer  leftDetB, rightDetB, upDetB, downDetB, oriDetB, failB, succB;
	sf::Sound leftDetS, rightDetS, upDetS, downDetS, oriDetS, failS, succS;
	//cv::Ptr<cv::BackgroundSubtractor> pBackSub;
	//cv::Mat getMedian(std::vector<cv::Mat> vec);
	//int calcMedian(vector<int>elements);
};