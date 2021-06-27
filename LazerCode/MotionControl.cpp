#include "pch.h"
#include "objDet.h"

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080
using namespace cv;

CascadeClassifier face_cascade;
CascadeClassifier hand_cascade;

objDet::objDet(int argc, const char** argv)
{
	justAttacked = false;
	calibrationArray = { false };

	int camera_device = 0;
	setFPS(camera_device);
	
	

	pregameBGIMG.loadFromFile("Assets/Menus/menuBack.png");

	if (!pregameBGTexture.loadFromImage(pregameBGIMG))
		errorMessageExit("ERROR: pregame background texture failed to load");

	pregameBGSprite.setTexture(pregameBGTexture);

	CommandLineParser parser(argc, argv,
		"{face_cascade_TRAINED|Assets/Cascade/FaceCascade.xml|Path to face cascade.}"
		"{hand_cascade|Assets/Cascade/HandCascade.xml|Path to hand cascade.}");

	cv::String face_cascade_name = samples::findFile(parser.get<cv::String>("face_cascade_TRAINED"));
	cv::String hand_cascade_name = samples::findFile(parser.get<cv::String>("hand_cascade"));

	//-- 1. Load the cascades
	if (!face_cascade.load(parser.get<cv::String>("face_cascade_TRAINED")))
		errorMessageExit("--(!)Error loading face cascade");

	if (!hand_cascade.load(parser.get<cv::String>("hand_cascade")))
		errorMessageExit("--(!)Error loading hand cascade");


	if (!leftDetB.loadFromFile("Assets/Audio/Left_Detected.wav") ||
		!rightDetB.loadFromFile("Assets/Audio/Right_Detected.wav") ||
		!upDetB.loadFromFile("Assets/Audio/Up_Detected.wav") ||
		!downDetB.loadFromFile("Assets/Audio/Down_Detected.wav") ||
		!oriDetB.loadFromFile("Assets/Audio/Origin_Detected.wav") ||
		!failB.loadFromFile("Assets/Audio/Failure.wav") ||
		!succB.loadFromFile("Assets/Audio/Success.wav") ||
		!switchToKeyboardB.loadFromFile("Assets/Audio/keyboard_switch.wav") ||
		!switchToMotionB.loadFromFile("Assets/Audio/motion_switch.wav"))
		errorMessageExit("Sound Load Fail");

	leftDetS.setBuffer(leftDetB); rightDetS.setBuffer(rightDetB); upDetS.setBuffer(upDetB);
	downDetS.setBuffer(downDetB); oriDetS.setBuffer(oriDetB); failS.setBuffer(failB); succS.setBuffer(succB);
	switchToKeyboardS.setBuffer(switchToKeyboardB);
	switchToMotionS.setBuffer(switchToMotionB);

	/*---TEXT---*/

	if (!font_ponde.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");

	pregame_origin_text.setFont(font_ponde);
	pregame_up_text.setFont(font_ponde);
	pregame_down_text.setFont(font_ponde);
	pregame_left_text.setFont(font_ponde);
	pregame_right_text.setFont(font_ponde);


	pregame_origin_text.setString("PRESS 'O' TO SET YOUR ORIGIN POINT");
	pregame_up_text.setString("UP");
	pregame_down_text.setString("DOWN");
	pregame_left_text.setString("LEFT");
	pregame_right_text.setString("RIGHT");


	pregame_origin_text.setCharacterSize(60);
	pregame_up_text.setCharacterSize(60);
	pregame_down_text.setCharacterSize(60);
	pregame_left_text.setCharacterSize(60);
	pregame_right_text.setCharacterSize(60);

	pregame_origin_text.setFillColor(sf::Color::White);
	pregame_up_text.setFillColor(sf::Color::Red);
	pregame_down_text.setFillColor(sf::Color::Red);
	pregame_left_text.setFillColor(sf::Color::Red);
	pregame_right_text.setFillColor(sf::Color::Red);


	pregame_origin_text.setPosition(100, 150);
	pregame_up_text.setPosition(930, 150);
	pregame_down_text.setPosition(900, 800);
	pregame_left_text.setPosition(350, 500);
	pregame_right_text.setPosition(1400, 500);

}


std::array<bool, 5> objDet::runMotionDetect(std::array<bool, 5> userInput)
{
	Mat frame, frameInv, cropFrame;

	capture.read(frameInv);
	cv::flip(frameInv, frame, 1);

	if (frame.empty())
	{
		message("--(!)Error opening video capture", "ERROR");
		return { false, false, false, false, true };
	}

	//-- 3. Apply the classifier to the frame
	Mat frame_gray, frame_gray_CROPPED;
	GaussianBlur(frame, frame, Size(3, 3), 0);
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	std::vector<cv::Rect> faces;
	face_cascade.detectMultiScale(frame_gray, faces);

	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
		cv::rectangle(frame, originRect, Scalar(255, 0, 0), 2);
		motionDetectF.emplace_back(center);

		if (motionDetectF.size() == movementsPerSecond)
		{

			int displacementX = motionDetectF.at(movementsPerSecond - 1).x - originPoint.x;
			int displacementY = motionDetectF.at(movementsPerSecond - 1).y - originPoint.y;

			motionDetectF.resize(0);

			if ((displacementX > 15 || displacementX < -15) && abs(displacementX) > abs(displacementY))  //if the x movement is signifigant, record it
			{
				if (displacementX < -15)
					return std::array<bool, 5>{ false, false, true, false, false };

				else if (displacementX > 15)
					return std::array<bool, 5> { false, false, false, true, false };
			}

			else if ((displacementY > 15 || displacementY < -15) && abs(displacementX) < abs(displacementY)) //if the y movement is signifigant, record it
			{
				if (displacementY < -15)
					return std::array<bool, 5> { true, false, false, false, false };

				else if (displacementY > 15)
					return std::array<bool, 5> { false, true, false, false, false };
			}
			else
				return std::array<bool, 5> { false, false, false, false, true };

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
		errorMessageExit("ERROR: texture failed to load");

	videoSprite.setTexture(videoTexture);

	return userInput;
	//imshow("Frame", frame);

}

bool objDet::cameraSwitch(bool useKeyboard)
{
	int camera_device = 0;
	//capture.open(camera_device);
	//-- 2. Read the video stream
	if (useKeyboard == false && !capture.isOpened()) //if the user wants to use the motion controls and isnt using motion controls already
	{
		capture.open(camera_device);
		if (capture.isOpened())
		{
			//setFPS();
			switchToMotionS.play();
			return false;
		}
		else
		{
			switchToKeyboardS.play();
			return true;
		}

		//errorMessageExit("--(!)Error opening video capture");

	}
	else if (useKeyboard == true)
	{
		switchToKeyboardS.play();
		//cout << "USING DEFAULT KEYBOARD CONTROLS" << endl;
		if (capture.isOpened())
			capture.release();
		return true;
	}
	else
		return useKeyboard;


}

bool objDet::calibrate(sf::RenderWindow& _window)
{
	Mat frame, frameInv, cropFrame;

	if (!calibrationArray[0] || !calibrationArray[1] || !calibrationArray[2] || !calibrationArray[3] || !calibrationArray[4])
	{
		_window.clear();
		_window.draw(pregameBGSprite);

		capture.read(frameInv);
		cv::flip(frameInv, frame, 1);

		if (frame.empty())
			errorMessageExit("--(!) No captured frame -- Break!");

		//-- 3. Apply the classifier to the frame
		Mat frame_gray, frame_gray_CROPPED;
		GaussianBlur(frame, frame, Size(3, 3), 0);
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//-- Detect faces
		std::vector<cv::Rect> faces;
		face_cascade.detectMultiScale(frame_gray, faces);


		for (int i = 0; i < faces.size(); i++)
		{
			Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);

			if (!calibrationArray[4] && sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			{
				oriDetS.play();
				pregame_origin_text.setFillColor(sf::Color::Green);
				_window.draw(pregame_origin_text);
				_window.draw(videoSprite);
				_window.display();

				Sleep(1200);
				calibrationArray[4] = true;
				originPoint = center;
				originRect = faces[i];
			}
			else if (!calibrationArray[4])
				_window.draw(pregame_origin_text);

			cv::ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
			cv::rectangle(frame, originRect, Scalar(255, 0, 0), 3);

			if (motionDetectF.size() < movementsPerSecond && calibrationArray[4])
				motionDetectF.emplace_back(center);

			if (motionDetectF.size() == movementsPerSecond && calibrationArray[4])
			{
				int displacementX = motionDetectF.at(movementsPerSecond - 1).x - originPoint.x;
				int displacementY = motionDetectF.at(movementsPerSecond - 1).y - originPoint.y;

				motionDetectF.resize(0);

				if ((displacementX > 15 || displacementX < -15) && abs(displacementX) > abs(displacementY)) //if the movement is signifigant, record it
				{
					if (displacementX < -15 && !calibrationArray[2])
					{
						leftDetS.play();
						calibrationArray[2] = true; //left calibrated
						pregame_left_text.setFillColor(sf::Color::Green);
					}

					else if (displacementX > 15 && !calibrationArray[3])
					{
						rightDetS.play();
						calibrationArray[3] = true;
						pregame_right_text.setFillColor(sf::Color::Green);
					}

				}

				else if ((displacementY > 15 || displacementY < -15) && abs(displacementX) < abs(displacementY))
				{
					if (displacementY < -15 && !calibrationArray[0])
					{
						upDetS.play();
						calibrationArray[0] = true;
						pregame_up_text.setFillColor(sf::Color::Green);
					}

					else if (displacementY > 15 && !calibrationArray[1])
					{
						downDetS.play();
						calibrationArray[1] = true;
						pregame_down_text.setFillColor(sf::Color::Green);
					}
				}


			}
		}

		cvtColor(frame, frame, cv::COLOR_BGR2RGBA);
		videoImg.create(frame.cols, frame.rows, frame.ptr());

		if (!videoTexture.loadFromImage(videoImg))
			errorMessageExit("ERROR: texture failed to load");

		videoSprite.setTexture(videoTexture);
		videoSprite.setPosition(650, 275);

		if (calibrationArray[4])
		{
			_window.draw(pregame_left_text);
			_window.draw(pregame_right_text);
			_window.draw(pregame_up_text);
			_window.draw(pregame_down_text);
		}

		_window.draw(videoSprite);
		_window.display();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) // escape
		{
			calibrationArray = { false };
			failS.play();
			return false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) // escape
			exit(EXIT_FAILURE);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
			return true;

		return false;
	}

	Sleep(1200);
	succS.play();
	calibrationArray = { false };
	return true;
}

void objDet::detectMotionHand(cv::Mat frame)
{
	int radiusChange = scaleInc[1] - scaleInc[0];

	if (radiusChange > 15 && !justAttacked) //attack
	{
		//	system("cls");
		//	cout << "ATTACK" << endl;
		justAttacked = true;

	}

	else if (radiusChange < 15) //character goes back to stand by
	{
		//	system("cls");
		//	cout << "STAND BY" << endl;
		justAttacked = false;
	}
}

void objDet::setFPS(int cam_device)
{
	VideoCapture grabFPS(cam_device);


	// Number of frames to capture
	int num_frames = 120;

	// Start and end times
	time_t start, end;

	// Variable for storing video frames
	Mat frame;

	//cout << "Capturing " << num_frames << " frames" << endl;

	// Start time
	time(&start);

	// Grab a few frames
	for (int i = 0; i < num_frames; i++)
	{
		grabFPS >> frame;
	}

	// End Time
	time(&end);

	// Time elapsed
	double seconds = difftime(end, start);
	cout << "Time taken : " << seconds << " seconds" << endl;

	// Calculate frames per second
	fps = (int)(num_frames / seconds);
	movementsPerSecond = fps / 3;
	cout << "Estimated frames per second : " << fps << endl;

	grabFPS.release();

}
