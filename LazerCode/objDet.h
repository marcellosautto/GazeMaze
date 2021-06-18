#pragma once

#include "pch.h"


class objDet {

public:

	objDet(int argc, const char** argv);

	//driver
	std::array<bool, 5> runMotionDetect();

	std::array<bool, 5> detectMotionDirectionFace(cv::Mat frame);
	bool calibrate(sf::RenderWindow& _window);
	void detectMotionHand(cv::Mat frame);

	
	sf::Sprite videoSprite;


private:

	cv::VideoCapture capture;

	char lastDirectionF;
	bool justAttacked;
	cv::Rect originRect;

	sf::Image videoImg;
	sf::Texture videoTexture;

	std::vector<cv::Point2d> motionDetectF;
	std::vector<int> scaleInc;

	std::array<bool, 5> movements;
	std::array<bool, 5> calibrationArray;

	//cv::Ptr<cv::BackgroundSubtractor> pBackSub;
	//cv::Mat getMedian(std::vector<cv::Mat> vec);
	//int calcMedian(vector<int>elements);
};
