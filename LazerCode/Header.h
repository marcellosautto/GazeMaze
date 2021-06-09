#pragma once

#include "pch.h"

class source {

public:
	char lastDirection;

	std::vector<cv::Point2d> motionDetect;
	void detectAndDisplay(cv::Mat frame);
	void detectMotionDirection();
	int runOpenCV(int argc, const char** argv, source &s);
};
