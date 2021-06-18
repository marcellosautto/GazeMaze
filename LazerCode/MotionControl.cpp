#include "pch.h"
#include "objDet.h"

using namespace cv;

CascadeClassifier face_cascade;
CascadeClassifier hand_cascade;

objDet::objDet(int argc, const char** argv)
{
	justAttacked = false;
	calibrationArray = { false };

	CommandLineParser parser(argc, argv,
		"{face_cascade_TRAINED|Assets/cascadeFace/cascade.xml|Path to face cascade.}"
		"{hand_cascade|Assets/cascadeHand/palm_v4.xml|Path to hand cascade.}"
		"{camera|0|Camera device number.}");

	cv::String face_cascade_name = samples::findFile(parser.get<cv::String>("face_cascade_TRAINED"));
	cv::String hand_cascade_name = samples::findFile(parser.get<cv::String>("hand_cascade"));

	//-- 1. Load the cascades
	if (!face_cascade.load(parser.get<cv::String>("face_cascade_TRAINED")))
	{
		cout << "--(!)Error loading face cascade\n";
		exit(EXIT_FAILURE);
	};
	if (!hand_cascade.load(parser.get<cv::String>("hand_cascade")))
	{
		cout << "--(!)Error loading hand cascade\n";
		exit(EXIT_FAILURE);
	};
	int camera_device = parser.get<int>("camera");

	//-- 2. Read the video stream
	capture.open(camera_device);

	if (!capture.isOpened())
	{
		cout << "--(!)Error opening video capture\n";
		exit(EXIT_FAILURE);
	}

}


std::array<bool, 5> objDet::runMotionDetect()
{

	Mat frame, frameInv;

	capture.read(frameInv);
	cv::flip(frameInv, frame, 1);

	if (frame.empty())
	{
		cout << "--(!) No captured frame -- Break!\n";
		return { false, false, false, false, true };
	}
	//-- 3. Apply the classifier to the frame
	//cv::rectangle(frame, Point(50, 50), Point(300, 300), Scalar(0, 255, 0), 2);
	//cv::Mat cropImg = frame(Range(50, 300), Range(50, 300));
	//detectAndDisplay(frame, cropImg);
	Mat frame_gray, frame_gray_CROPPED;
	GaussianBlur(frame, frame, Size(3, 3), 0);
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//cvtColor(cropImg, frame_gray_CROPPED, COLOR_BGR2GRAY);
	//equalizeHist(frame_gray_CROPPED, frame_gray_CROPPED);

	//-- Detect faces
	std::vector<cv::Rect> faces;
	face_cascade.detectMultiScale(frame_gray, faces);

	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 125, 0), 4); 
		rectangle(frame, originRect, Scalar(255, 0, 255), 3, 1);
		motionDetectF.emplace_back(center);
		if (motionDetectF.size() == 20)
		{

			int displacementX = motionDetectF.at(19).x - motionDetectF.at(0).x;
			int displacementY = motionDetectF.at(19).y - motionDetectF.at(0).y;

			motionDetectF.resize(motionDetectF.size() - 10);

			if (displacementX > 15 || displacementX < -15) //if the movement is signifigant, record it
			{
				if (displacementX < -15/* && lastDirectionF != 'L'*/)
				{

					lastDirectionF = 'L';
					system("cls");
					cout << "LEFT" << endl;
					return std::array<bool, 5>{ false, false, true, false, false };
				}

				else if (displacementX > 15 /*&& lastDirectionF != 'R'*/)
				{
					lastDirectionF = 'R';
					system("cls");
					cout << "RIGHT" << endl;
					return std::array<bool, 5> { false, false, false, true, false };
				}

			}

			else if (displacementY > 15 || displacementY < -15)
			{
				if (displacementY < -15 /*&& lastDirectionF != 'U'*/)
				{
					lastDirectionF = 'U';
					system("cls");
					cout << "UP" << endl;
					return std::array<bool, 5> { true, false, false, false, false };
				}

				else if (displacementY > 15 /*&& lastDirectionF != 'D'*/)
				{
					lastDirectionF = 'D';
					system("cls");
					cout << "DOWN" << endl;
					return std::array<bool, 5> { false, true, false, false, false };
				}
			}
			else
			{
				lastDirectionF = 0;
				system("cls");
				cout << "AT ORIGIN" << endl;
				return std::array<bool, 5> { false, false, false, false, true };
			}



		}

	}

	//-- Detect hand
	/*std::vector<cv::Rect> hands;
	hand_cascade.detectMultiScale(frame_gray_CROPPED, hands);

	for (int i = 0; i < hands.size(); i++)
	{
		Point center(hands[i].x + hands[i].width / 2, hands[i].y + hands[i].height / 2);
		int radius = cvRound((hands[i].width + hands[i].height) * 0.25);
		circle(cropImg, center, radius, Scalar(255, 0, 0), 4);

		scaleInc.emplace_back(radius);
		if (scaleInc.size() == 2)
		{
			detectMotionHand(cropImg);
			scaleInc.resize(scaleInc.size() - 1);
		}
	}*/

	//-- Show what you got
	//imshow("Capture Hand Detect", crop);

	cvtColor(frame, frame, cv::COLOR_BGR2RGBA);
	videoImg.create(frame.cols, frame.rows, frame.ptr());

	if (!videoTexture.loadFromImage(videoImg))
	{
		cout << "ERROR: texture failed to load\n";
		exit(EXIT_FAILURE);
	}

	videoSprite.setTexture(videoTexture);
	

	//imshow("Frame", frame);

}


bool objDet::calibrate(sf::RenderWindow& _window)
{
	Mat frame, frameInv;

	while (!calibrationArray[0] || !calibrationArray[1] || !calibrationArray[2] || !calibrationArray[3] || !calibrationArray[4])
	{

		capture.read(frameInv);
		cv::flip(frameInv, frame, 1);

		if (frame.empty())
		{
			cout << "--(!) No captured frame -- Break!\n";
			exit(EXIT_FAILURE);
		}
		//-- 3. Apply the classifier to the frame
		//cv::rectangle(frame, Point(50, 50), Point(300, 300), Scalar(0, 255, 0), 2);
		//cv::Mat cropImg = frame(Range(50, 300), Range(50, 300));
		//detectAndDisplay(frame, cropImg);
		Mat frame_gray, frame_gray_CROPPED;
		GaussianBlur(frame, frame, Size(3, 3), 0);
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//cvtColor(cropImg, frame_gray_CROPPED, COLOR_BGR2GRAY);
		//equalizeHist(frame_gray_CROPPED, frame_gray_CROPPED);

		//-- Detect faces
		std::vector<cv::Rect> faces;
		face_cascade.detectMultiScale(frame_gray, faces);

		for (int i = 0; i < faces.size(); i++)
		{
			Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
			ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
			motionDetectF.emplace_back(center);
			if (motionDetectF.size() == 20)
			{

				int displacementX = motionDetectF.at(19).x - motionDetectF.at(0).x;
				int displacementY = motionDetectF.at(19).y - motionDetectF.at(0).y;

				motionDetectF.resize(motionDetectF.size() - 10);

				if (displacementX > 15 || displacementX < -15) //if the movement is signifigant, record it
				{
					if (displacementX < -15 && !calibrationArray[2])
					{

						calibrationArray[2] = PlaySound(TEXT("Assets/Audio/Left_Detected.wav"), NULL, SND_SYNC); //left calibrated
						//system("cls");
						cout << "LEFT CALIBRATED" << endl;
					}

					else if (displacementX > 15 && !calibrationArray[3])
					{
						calibrationArray[3] = PlaySound(TEXT("Assets/Audio/Right_Detected.wav"), NULL, SND_SYNC);
						//system("cls");
						cout << "RIGHT CALIBRATED" << endl;

					}

				}

				else if (displacementY > 15 || displacementY < -15)
				{
					if (displacementY < -15 && !calibrationArray[0])
					{
						calibrationArray[0] = PlaySound(TEXT("Assets/Audio/Up_Detected.wav"), NULL, SND_SYNC);
						//system("cls");
						cout << "UP CALIBRATED" << endl;

					}

					else if (displacementY > 15 && !calibrationArray[1])
					{
						calibrationArray[1] = PlaySound(TEXT("Assets/Audio/Down_Detected.wav"), NULL, SND_SYNC);
						//system("cls");
						cout << "DOWN CALIBRATED" << endl;

					}
				}
				else if (!calibrationArray[4])
				{
					calibrationArray[4] = PlaySound(TEXT("Assets/Audio/Origin_Detected.wav"), NULL, SND_SYNC);
					//system("cls");
					cout << "ORIGIN CALIBRATED" << endl;

					originRect = faces[i];
					rectangle(frame, originRect, Scalar(255, 125, 0), 3, 1);

				}

			}
		}


	
		//imshow("PREGAME CHECK- Press 'ESC' If Configuration Errors Occur", frame);
		cvtColor(frame, frame, cv::COLOR_BGR2RGBA);
		videoImg.create(frame.cols, frame.rows, frame.ptr());

		if (!videoTexture.loadFromImage(videoImg))
		{
			cout << "ERROR: texture failed to load\n";
			exit(EXIT_FAILURE);
		}

		videoSprite.setTexture(videoTexture);
		videoSprite.setPosition(540, 240);
		sf::RenderTarget& renderTarget = _window;
		renderTarget.draw(videoSprite);
		_window.display();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) // escape
		{
			//destroyWindow("PREGAME CHECK- Press 'ESC' If Configuration Errors Occur");
			//_window.close();
			calibrationArray = { false };
			return false;
			//break; 
		}
		_window.clear();
	}

	//destroyWindow("PREGAME CHECK- Press 'ESC' If Configuration Errors Occur");
	return true;
}

std::array<bool, 5> objDet::detectMotionDirectionFace(cv::Mat frame)
{
	int displacementX = motionDetectF.at(1).x - motionDetectF.at(0).x;
	int displacementY = motionDetectF.at(1).y - motionDetectF.at(0).y;

	if ((displacementX > 15 || displacementX < -15)) //if the movement is signifigant, record it
	{
		if (displacementX < -15 && lastDirectionF != 'L')
		{

			lastDirectionF = 'L';
			system("cls");
			cout << "LEFT" << endl;
			return { false, false, true, false };
		}

		else if (displacementX > 15 && lastDirectionF != 'R')
		{
			lastDirectionF = 'R';
			system("cls");
			cout << "RIGHT" << endl;
			return { false, false, false, true };
		}

	}

	else if ((displacementY > 15 || displacementY < -15))
	{
		if (displacementY < -15 && lastDirectionF != 'U')
		{
			lastDirectionF = 'U';
			system("cls");
			cout << "UP" << endl;
			return { true, false, false, false };
		}

		else if (displacementY > 15 && lastDirectionF != 'D')
		{
			lastDirectionF = 'D';
			system("cls");
			cout << "DOWN" << endl;
			return { false, true, false, false };
		}
	}
	else
	{
		lastDirectionF = 0;
		system("cls");
		cout << "AT ORIGIN" << endl;

		return { false, false, false, false };
	}



}

void objDet::detectMotionHand(cv::Mat frame)
{
	int radiusChange = scaleInc[1] - scaleInc[0];

	if (radiusChange > 15 && !justAttacked) //attack
	{
		system("cls");
		cout << "ATTACK" << endl;
		justAttacked = true;

	}

	else if (radiusChange < 15) //character goes back to stand by
	{
		system("cls");
		cout << "STAND BY" << endl;
		justAttacked = false;
	}



}

