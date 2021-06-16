#include "pch.h"
#include "objDet.h"

using namespace cv;

CascadeClassifier face_cascade;


int objDet::runOpenCVFaceDetect(int argc, const char** argv, objDet&od)
{
	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{face_cascade_TRAINED|Assets/cascade/cascade.xml|Path to face cascade.}"
		"{camera|0|Camera device number.}");
	parser.about("\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face + eyes) in a video stream.\n"
		"You can use Haar or LBP features.\n\n");
	parser.printMessage();

	cv::String face_cascade_name = samples::findFile(parser.get<cv::String>("face_cascade_TRAINED"));

	//-- 1. Load the cascades
	if (!face_cascade.load(parser.get<cv::String>("face_cascade_TRAINED")))
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
		od.detectAndDisplayHand(frame);
		if (waitKey(10) == 27)
		{
			break; // escape
		}
	}
	return 0;
}

void objDet::detectAndDisplayFace(Mat frame)
{
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	std::vector<cv::Rect> faces;
	//faces.resize(1);
	face_cascade.detectMultiScale(frame_gray, faces);

	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
		motionDetectF.emplace_back(center);
		if (motionDetectF.size() == 2)
		{
			detectAndDisplayFace(frame);
			motionDetectF.resize(motionDetectF.size() - 1);
		}
	}
	
	//-- Show what you got
	imshow("Capture - Face detection", frame);
}

void objDet::detectMotionDirectionFace(cv::Mat frame)
{
	int displacementX = motionDetectF.at(1).x - motionDetectF.at(0).x;
	int displacementY = motionDetectF.at(1).y - motionDetectF.at(0).y;

	if (displacementX > 15 || displacementX < -15) //if the movement is signifigant, record it
	{
		if (displacementX < -20 && lastDirectionF != 'L')
		{
			lastDirectionF = 'L';
			system("cls");
			cout << "LEFT" << endl;
			//cv::putText(frame, "LEFT", cv::Size(50,50), cv::FONT_HERSHEY_COMPLEX,
				//1, cv::Scalar(255,0,0), 2, cv::LINE_AA);
		}

		else if (displacementX > 15 && lastDirectionF != 'R')
		{
			lastDirectionF = 'R';
			system("cls");
			cout << "RIGHT" << endl;
		}

	}

	else if (displacementY > 15 || displacementY < -15)
	{
		if (displacementY < -15 && lastDirectionF != 'U')
		{
			lastDirectionF = 'U';
			system("cls");
			cout << "UP" << endl;
		}

		else if (displacementY > 15 && lastDirectionF != 'D')
		{
			lastDirectionF = 'D';
			system("cls");
			cout << "DOWN" << endl;
		}
	}
	else
	{
		lastDirectionF = 0;
		system("cls");
		cout << "AT ORIGIN" << endl;
	}



}
