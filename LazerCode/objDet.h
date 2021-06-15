#pragma once

#include "pch.h"

class objDet {

public:
	objDet();

	char lastDirectionF, lastDirectionH;
	std::vector<cv::Point2d> motionDetectF;
	std::vector<cv::Point2d> motionDetectH;

	cv::Ptr<cv::BackgroundSubtractor> pBackSub;

	void detectAndDisplayFace(cv::Mat frame);
	void detectMotionDirectionFace(cv::Mat frame);
	int runOpenCVFaceDetect(int argc, const char** argv, objDet&od);

	void detectAndDisplayHand(cv::Mat frame);
	void detectMotionDirectionHand();
	int runOpenCVHandDetect(int argc, const char** argv, objDet& od);

	cv::Mat getMedian(std::vector<cv::Mat> vec);
	int calcMedian(vector<int>elements);
};
