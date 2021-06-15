#include "pch.h"
#include "objDet.h"

using namespace cv;

objDet::objDet()
{
	pBackSub = createBackgroundSubtractorMOG2();
}

int objDet::runOpenCVHandDetect(int argc, const char** argv, objDet& od)
{
	cv::VideoCapture vc(0);
	cv::Mat frame, frameInv;


	vc.open(0);
	if (!vc.isOpened())
	{
		cout << "--(!)Error opening video capture\n";
		return -1;
	}


	while (vc.read(frameInv))
	{
		cv::flip(frameInv, frame, 1);

		if (frame.empty())
		{
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}
		//-- 3. Apply the classifier to the frame
		od.detectAndDisplayHand(frame);
		if (waitKey(10) == 27)
		{
			break; // escape
		}
	}
	return 0;
}
void objDet::detectAndDisplayHand(Mat frame)
{
	cv::Mat frameBlurred, frameHSV, edges, croppedFrameGRAY, frameTH, medianFrameGRAY, diffFrame;
	double thresh = 175;
	double maxValue = 255;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	cv::rectangle(frame, Point(100, 100), Point(300, 300), Scalar(255, 0, 255), 2);


	cv::Mat croppedFrameMask;

	//update the background model
	cv::Mat cropImg = frame(Range(100, 300), Range(100, 300));
	pBackSub->apply(cropImg, croppedFrameMask, -1);

	//show the current frame and the fg masks
	imshow("FG Mask", croppedFrameMask);

	//contour hand
	//cvtColor(croppedFrameMask, croppedFrameGRAY, COLOR_);
	threshold(croppedFrameMask, frameTH, thresh, maxValue, THRESH_BINARY);

	//imshow("Contour Test", frameTH);

	findContours(frameTH, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	Mat frameCpy = cropImg.clone();
	drawContours(cropImg, contours, -1, Scalar(0, 255, 0), 2);
	imshow("Normal Frame", frame);
	//imshow("Contoured Frame", cropImg);



	/*vector<cv::Mat> frames;

	for (int i = 0; i < 30; i++)
		frames.push_back(cropImg);


	// Calculate the median along the time axis

	Mat medianFrame = getMedian(frames);

	cvtColor(medianFrame, medianFrameGRAY, COLOR_RGB2GRAY);

	absdiff(croppedFrameGRAY, medianFrameGRAY, diffFrame);

	threshold(diffFrame, diffFrame, 30, 255, THRESH_BINARY);
	// Display difference frame
	imshow("frame", medianFrameGRAY);

	waitKey(20);*/


}

int objDet::calcMedian(vector<int> elements)
{
	nth_element(elements.begin(), elements.begin() + elements.size() / 2, elements.end());

	//sort(elements.begin(),elements.end());
	return elements[elements.size() / 2];
}

cv::Mat objDet::getMedian(std::vector<cv::Mat> vec)
{
	// Note: Expects the image to be CV_8UC3
	cv::Mat medianImg(vec[0].rows, vec[0].cols, CV_8UC3, cv::Scalar(0, 0, 0));

	for (int row = 0; row < vec[0].rows; row++)
	{
		for (int col = 0; col < vec[0].cols; col++)
		{
			std::vector<int> elements_B;
			std::vector<int> elements_G;
			std::vector<int> elements_R;

			for (int imgNumber = 0; imgNumber < vec.size(); imgNumber++)
			{
				int R = vec[imgNumber].at<cv::Vec3b>(row, col)[0];
				int G = vec[imgNumber].at<cv::Vec3b>(row, col)[1];
				int B = vec[imgNumber].at<cv::Vec3b>(row, col)[2];

				elements_R.push_back(R);
				elements_G.push_back(G);
				elements_B.push_back(B);


			}
			medianImg.at<cv::Vec3b>(row, col)[0] = calcMedian(elements_R);
			medianImg.at<cv::Vec3b>(row, col)[1] = calcMedian(elements_G);
			medianImg.at<cv::Vec3b>(row, col)[2] = calcMedian(elements_B);

		}
	}
	return medianImg;
}

void objDet::detectMotionDirectionHand()
{

}
