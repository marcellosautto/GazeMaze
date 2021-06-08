#pragma once

#include "pch.h"

class source {

public:
	char lastDirection;

	std::vector<cv::Point2d> motionDetect;
	void detectAndDisplay(Mat frame);
	void detectMotionDirection();
};
