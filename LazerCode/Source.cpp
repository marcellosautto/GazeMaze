#include "pch.h"
#include "Header.h"

using namespace cv;

CascadeClassifier face_cascade;

int source::runOpenCV(int argc, const char** argv, source &s)
{
	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{face_cascade|Assets/xmlFiles/haarcascades/haarcascade_frontalface_alt.xml|Path to face cascade.}"
		"{camera|0|Camera device number.}");
	parser.about("\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face + eyes) in a video stream.\n"
		"You can use Haar or LBP features.\n\n");
	parser.printMessage();

	cv::String face_cascade_name = samples::findFile(parser.get<cv::String>("face_cascade"));

	//-- 1. Load the cascades
	if (!face_cascade.load(parser.get<cv::String>("face_cascade")))
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

	while (capture.read(frame))
	{
		//cv::flip(frame, frameInv, 1);

		//rectangle(frameInv, p1, p2, Scalar(255, 0, 0), 2, 0, 0);

		if (frame.empty())
		{
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}
		//-- 3. Apply the classifier to the frame
		s.detectAndDisplay(frame);
		if (waitKey(10) == 27)
		{
			break; // escape
		}
	}
	return 0;
}
void source::detectAndDisplay(Mat frame)
{
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	std::vector<cv::Rect> faces;
	faces.resize(1);
	face_cascade.detectMultiScale(frame_gray, faces);

	Point center(faces[0].x + faces[0].width / 2, faces[0].y + faces[0].height / 2);
	ellipse(frame, center, Size(faces[0].width / 2, faces[0].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
	motionDetect.emplace_back(center);
	if (motionDetect.size() == 2)
	{
		detectMotionDirection();
		motionDetect.resize(motionDetect.size() - 1);
	}
	//-- Show what you got
	imshow("Capture - Face detection", frame);
}

void source::detectMotionDirection()
{
	int displacementX = motionDetect.at(1).x - motionDetect.at(0).x;
	int displacementY = motionDetect.at(1).y - motionDetect.at(0).y;

	if (displacementX > 50 || displacementX < -50) //if the movement is signifigant, record it
	{
		if (displacementX < -50 && lastDirection != 'L')
		{
			lastDirection = 'L';
			system("cls");
			cout << "LEFT" << endl;
		}

		else if (displacementX > 50 && lastDirection != 'R')
		{
			lastDirection = 'R';
			system("cls");
			cout << "RIGHT" << endl;
		}

	}

	else if (displacementY > 50 || displacementY < -50)
	{
		if (displacementY < -50 && lastDirection != 'U')
		{
			lastDirection = 'U';
			system("cls");
			cout << "UP" << endl;
		}

		else if (displacementY > 50 && lastDirection != 'D')
		{
			lastDirection = 'D';
			system("cls");
			cout << "DOWN" << endl;
		}
	}
	else
	{
		lastDirection = '0';
		system("cls");
		cout << "AT ORIGIN" << endl;
	}



}
