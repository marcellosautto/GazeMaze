#pragma once

#include "pch.h"

class objDet {

public:

	objDet(int argc, const char** argv);

	//driver
	std::array<bool, 5> runMotionDetect();

	std::array<bool, 5> detectMotionDirectionFace(cv::Mat frame);
	void detectMotionHand(cv::Mat frame);
	void detectAndDisplay(cv::Mat frame, cv::Mat crop);

	


private:

	cv::VideoCapture capture;

	char lastDirectionF;
	bool justAttacked;

	std::vector<cv::Point2d> motionDetectF;
	std::vector<int> scaleInc;

	std::array<bool, 5> movements;

	//cv::Ptr<cv::BackgroundSubtractor> pBackSub;
	//cv::Mat getMedian(std::vector<cv::Mat> vec);
	//int calcMedian(vector<int>elements);
};
