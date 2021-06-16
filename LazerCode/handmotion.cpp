#include "pch.h"
#include "objDet.h"

using namespace cv;

CascadeClassifier hand_cascade;

objDet::objDet()
{
	justAttacked = false;
}

int objDet::runOpenCVHandDetect(int argc, const char** argv, objDet& od)
{
	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{hand_cascade|Assets/cascadeHand/palm_v4.xml|Path to hand cascade.}"
		"{camera|0|Camera device number.}");
	parser.about("\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face + eyes) in a video stream.\n"
		"You can use Haar or LBP features.\n\n");
	parser.printMessage();

	cv::String hand_cascade_name = samples::findFile(parser.get<cv::String>("hand_cascade"));

	//-- 1. Load the cascades
	if (!hand_cascade.load(parser.get<cv::String>("hand_cascade")))
	{
		cout << "--(!)Error loading face cascade\n";
		return -1;
	};
	int camera_device = parser.get<int>("camera");
	VideoCapture capture;
	//-- 2. Read the video stream
	capture.open(camera_device);

	if (!capture.isOpened())
	{
		cout << "--(!)Error opening video capture\n";
		return -1;
	}

	Mat frame, frameInv;

	while (capture.read(frameInv))
	{
		cv::flip(frameInv, frame, 1);

		//rectangle(frameInv, p1, p2, Scalar(255, 0, 0), 2, 0, 0);

		if (frame.empty())
		{
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}
		//-- 3. Apply the classifier to the frame
		cv::Mat cropImg = frame(Range(50, 300), Range(50, 300));
		od.detectAndDisplayHand(cropImg);
		if (waitKey(10) == 27)
		{
			break; // escape
		}
	}
	return 0;
}
void objDet::detectAndDisplayHand(cv::Mat frame)
{
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect hand
	std::vector<cv::Rect> hands;
	//faces.resize(1);
	hand_cascade.detectMultiScale(frame_gray, hands);

	for (int i = 0; i < hands.size(); i++)
	{
		Point center(hands[i].x + hands[i].width / 2, hands[i].y + hands[i].height / 2);
		int radius = cvRound((hands[i].width + hands[i].height) * 0.25);
		circle(frame, center, radius, Scalar(255, 0, 0), 4);

		scaleInc.emplace_back(radius);
		if (scaleInc.size() == 2)
		{
			detectMotionHand(frame);
			scaleInc.resize(scaleInc.size() - 1);
		}
	}

	//-- Show what you got
	imshow("Capture - Face detection", frame);
}

void objDet::detectMotionHand(cv::Mat frame)
{
	int radiusChange = scaleInc[1] - scaleInc[0];

	if (radiusChange > 15 && !justAttacked) //if the movement is signifigant, record it
	{
		system("cls");
		cout << "ATTACK" << endl;
		justAttacked = true;
		//cv::putText(frame, "LEFT", cv::Size(50,50), cv::FONT_HERSHEY_COMPLEX,
			//1, cv::Scalar(255,0,0), 2, cv::LINE_AA);

	}

	else if (radiusChange < 15){
		system("cls");
		cout << "STAND BY" << endl;
		justAttacked = false;
		//cv::putText(frame, "LEFT", cv::Size(50,50), cv::FONT_HERSHEY_COMPLEX,
			//1, cv::Scalar(255,0,0), 2, cv::LINE_AA);
	}



}
