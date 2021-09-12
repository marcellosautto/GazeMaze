#pragma once

#include "pch.h"


class objDet {

public:
	objDet(int argc, const char** argv, bool& threadWaitT, string& waitMessageTT, bool keyboard);

	std::array<bool, 5> runMotionDetect(std::array<bool, 5> userInput, sf::RenderWindow& _window);

	bool cameraSwitch(bool useKeyboard), //switch camera on and off
		isCalibrated, //tells if motions controls are calibrated or not
		keyboard; //current user controls

	bool* threadWait = NULL;
	string* waitMessageT = NULL;

	void setFPS(int& movementsPerSecondRef), //sets movements per second based on camera fps
		resetCalibration(); //reset calibration variables

	sf::Sprite videoSprite; //contains sprite used to display video
	std::vector<cv::Point2d> motionDetectF; //holds frames in between movements
	cv::VideoCapture capture; //used to capture frames from camera

	//used for SFX when switching controls
	sf::SoundBuffer switchToKeyboardB, switchToMotionB;
	sf::Sound switchToKeyboardS, switchToMotionS;

	int movementsPerSecond;
	sf::Sprite pregameBGSprite; //background sprite

private:

	
	int fps, camera_device;

	cv::Rect originRect;
	cv::Point originPoint;

	sf::Font font_ponde;

	sf::Image videoImg;
	sf::Texture videoTexture, pregameBGTexture;

	std::array<bool, 5> movements, calibrationArray;

	sf::Text videoTxt,
		pregame_origin_text,
		pregame_up_text,
		pregame_down_text,
		pregame_left_text,
		pregame_right_text, 
		recal_txt;

	cv::Mat testFrame;

	sf::SoundBuffer  leftDetB, rightDetB, upDetB, downDetB, oriDetB, failB, succB;
	sf::Sound leftDetS, rightDetS, upDetS, downDetS, oriDetS, failS, succS;
};