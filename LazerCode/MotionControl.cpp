#include "pch.h"
#include "objDet.h"

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080
using namespace cv;

CascadeClassifier face_cascade;

/*
SOURCES

	(1) --- https://docs.opencv.org/master/db/d28/tutorial_cascade_classifier.html --- Line Blocks: 95-100, 111-139
	(2) --- https://docs.opencv.org/master/dc/d88/tutorial_traincascade.html
	(3) --- https://learnopencv.com/how-to-find-frame-rate-or-frames-per-second-fps-in-opencv-python-cpp/ ---Line Blocks: 304-322

*/

objDet::objDet(int argc, const char** argv, bool& threadWaitT, string& waitMessageTT, bool keyboardT)
{
	//initialize calibration variables
	calibrationArray = { false };
	isCalibrated = false;
	camera_device = 0; //0 -> primary camera device on computer
	keyboard = true; //user input set to keyboard controls by default
	
	//clear frame
	testFrame = cv::Mat::zeros(640, 480, CV_8UC3);

	//test if camera is present
	capture.open(camera_device);
	capture.grab();

	//if a camera is present, set default controls to motion controls
	if (keyboardT)
		keyboard = true;

	//if a camera is NOT present (frame is empty), set default controls to keyboard controls
	else if (capture.retrieve(testFrame, 0))
		keyboard = false;

	threadWait = &threadWaitT;
	waitMessageT = &waitMessageTT;

	/*SFML*/

	//Load background texture
	if (!pregameBGTexture.loadFromFile("Assets/Menus/menuBack.png"))
		errorMessageExit("ERROR: pregame background texture failed to load");
	pregameBGSprite.setTexture(pregameBGTexture);

	//Load and set audio files
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

	//SFML TEXT
	if (!font_ponde.loadFromFile("Assets/Font/ponde___.ttf"))
		errorMessageExit("Failed To Load Font");

	pregame_origin_text.setFont(font_ponde);
	pregame_up_text.setFont(font_ponde);
	pregame_down_text.setFont(font_ponde);
	pregame_left_text.setFont(font_ponde);
	pregame_right_text.setFont(font_ponde);
	recal_txt.setFont(font_ponde);

	pregame_origin_text.setString("PRESS 'O' TO SET YOUR ORIGIN POINT");
	pregame_up_text.setString("UP");
	pregame_down_text.setString("DOWN");
	pregame_left_text.setString("LEFT");
	pregame_right_text.setString("RIGHT");
	recal_txt.setString("PRESS 'Q' TO RESET CALBRATION");

	pregame_origin_text.setCharacterSize(60);
	pregame_up_text.setCharacterSize(60);
	pregame_down_text.setCharacterSize(60);
	pregame_left_text.setCharacterSize(60);
	pregame_right_text.setCharacterSize(60);
	recal_txt.setCharacterSize(60);

	pregame_origin_text.setFillColor(sf::Color::White);
	pregame_up_text.setFillColor(sf::Color::Red);
	pregame_down_text.setFillColor(sf::Color::Red);
	pregame_left_text.setFillColor(sf::Color::Red);
	pregame_right_text.setFillColor(sf::Color::Red);
	recal_txt.setFillColor(sf::Color::Red);

	pregame_origin_text.setPosition(100, 150);
	pregame_up_text.setPosition(930, 150);
	pregame_down_text.setPosition(900, 800);
	pregame_left_text.setPosition(350, 500);
	pregame_right_text.setPosition(1400, 500);
	recal_txt.setPosition(250, 50);

	//LOAD FACE CASCADE
	CommandLineParser parser(argc, argv, "{face_cascade_TRAINED|Assets/Cascade/FaceCascade.xml|Path to face cascade.}");
	cv::String face_cascade_name = samples::findFile(parser.get<cv::String>("face_cascade_TRAINED"));

	if (!face_cascade.load(parser.get<cv::String>("face_cascade_TRAINED")))
		errorMessageExit("--(!)Error loading face cascade");

}


std::array<bool, 5> objDet::runMotionDetect(std::array<bool, 5> userInput, sf::RenderWindow& _window)
{
	//1. Read frame from camera

	Mat frame, frameInv, cropFrame;
	Mat frame_gray, frame_gray_CROPPED;
	Point center;
	std::vector<cv::Rect> faces;

	capture.read(frameInv);
	cv::flip(frameInv, frame, 1);

	if (frame.empty())
	{
		message("--(!)Error opening video capture", "ERROR");
		return { false, false, false, false, true };
	}

	//2. Apply color changes
	GaussianBlur(frame, frame, Size(3, 3), 0); //gaussian blur to smooth out frame
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY); //create a gray copy of the frame for the face cascade to read
	equalizeHist(frame_gray, frame_gray);
	face_cascade.detectMultiScale(frame_gray, faces);//-- Detect faces

	//3. Draw face location
	for (int i = 0; i < faces.size(); i++)
	{
		center.x = faces[i].x + faces[i].width / 2;
		center.y = faces[i].y + faces[i].height / 2;

		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
		cv::rectangle(frame, originRect, Scalar(255, 0, 0), 2);
		if (motionDetectF.size() < movementsPerSecond && calibrationArray[4])
			motionDetectF.emplace_back(center);

		//4. Find displacement of face from the origin point
		if (motionDetectF.size() == movementsPerSecond && calibrationArray[4])
		{
			int displacementX = motionDetectF.at(movementsPerSecond - 1).x - originPoint.x;
			int displacementY = motionDetectF.at(movementsPerSecond - 1).y - originPoint.y;

			motionDetectF.resize(0);

			if (abs(displacementX) > abs(displacementY) && abs(displacementX) > 15)  //if the x movement is greater than the y movement and the movement is signifigant, record it
			{
				if (displacementX < -15) // LEFT DETECTED
				{
					if (!calibrationArray[2]) //Calibrate left
					{
						leftDetS.play();
						calibrationArray[2] = true;
						pregame_left_text.setFillColor(sf::Color::Green);
					}
					else if (isCalibrated) //Move left
						return std::array<bool, 5>{ false, false, true, false, false };
				}
				else if (displacementX > 15) //RIGHT DETECTED
				{
					if (!calibrationArray[3]) //Calibrate right
					{
						rightDetS.play();
						calibrationArray[3] = true;
						pregame_right_text.setFillColor(sf::Color::Green);
					}
					else if (isCalibrated) //Move right
						return std::array<bool, 5> { false, false, false, true, false };
				}
			}
			else if (abs(displacementY) > abs(displacementX) && abs(displacementY) > 15)   //if the y movement is greater than the x movement and the movement is signifigant, record it
			{
				if (displacementY < -15) //UP DETECTED
				{
					if (!calibrationArray[0]) //Calibrate up
					{
						upDetS.play();
						calibrationArray[0] = true;
						pregame_up_text.setFillColor(sf::Color::Green);
					}
					else if (isCalibrated) //Move up
						return std::array<bool, 5> { true, false, false, false, false };
				}
				else if (displacementY > 15) //DOWN DETECTED
				{
					if (!calibrationArray[1]) //Calibrate down
					{
						downDetS.play();
						calibrationArray[1] = true;
						pregame_down_text.setFillColor(sf::Color::Green);
					}
					else if (isCalibrated) //Move down
						return std::array<bool, 5> { false, true, false, false, false };
				}
			}
			else if (isCalibrated) //At origin
				return std::array<bool, 5> { false, false, false, false, true };
		}
	}

	//Draw Calibration Menu
	if (!isCalibrated)
	{
		videoSprite.setPosition(650, 275);
		_window.draw(pregameBGSprite);

		//Save origin point set by user in a cv::Rect
		if (!calibrationArray[4] && sf::Keyboard::isKeyPressed(sf::Keyboard::O))
		{
			oriDetS.play();
			pregame_origin_text.setFillColor(sf::Color::Green);
			_window.clear();
			_window.draw(pregameBGSprite);
			_window.draw(pregame_origin_text);
			_window.draw(videoSprite);
			_window.display();
			Sleep(1200);
			_window.clear();
			calibrationArray[4] = true;
			originPoint = center;
			originRect = faces[0];
		}

		//Draw text for registering left, right, up and down calibration once the origin is set
		if (calibrationArray[4])
		{
			_window.draw(recal_txt);
			_window.draw(pregame_left_text);
			_window.draw(pregame_right_text);
			_window.draw(pregame_up_text);
			_window.draw(pregame_down_text);
		}
		else
			_window.draw(pregame_origin_text);

		//Reset Motion Calibration
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) // escape
		{
			resetCalibration();
			failS.play();
			return std::array<bool, 5> { false };
		}
	}

	//Draw video camera
	cvtColor(frame, frame, cv::COLOR_BGR2RGBA);
	videoImg.create(frame.cols, frame.rows, frame.ptr());

	if (!videoTexture.loadFromImage(videoImg))
		errorMessageExit("ERROR: texture failed to load");
	videoSprite.setTexture(videoTexture);
	_window.draw(videoSprite);

	//once calibration is complete, allow player to play game
	if (calibrationArray[0] && calibrationArray[1] && calibrationArray[2] && calibrationArray[3] && calibrationArray[4] && !isCalibrated)
	{
		isCalibrated = true;
		_window.display();
		Sleep(1200);
		succS.play();
		return std::array<bool, 5> { true };
	}

	return userInput;
}

//Switches Controls and Camera On and Off
bool objDet::cameraSwitch(bool useKeyboard)
{
	if (!capture.isOpened())
		capture.open(camera_device);
	capture.grab();

	if (useKeyboard == false) //switch to motion controls
	{
		if (capture.retrieve(testFrame, 0)) //only switch to motion controls if a camera is present
		{
			thread sfpsThread(&objDet::setFPS, this, ref(movementsPerSecond)); //set movements per second in a different thread
			sfpsThread.detach();
			*threadWait = true;
			switchToMotionS.play();

			resetCalibration();

			return false;
		}
		else //set to keyboard controls if camera not detected
			return true;
	}

	else if (useKeyboard == true) //switch to keyboard controls, resets calibration
	{
		switchToKeyboardS.play();
		resetCalibration();
		if (capture.isOpened()) //close camera if open
			capture.release();

		return true;
	}
	else
		return useKeyboard;
}

//Set movements per second based on how fast your camera reads in frames
void objDet::setFPS(int& movementsPerSecondRef)
{
	*waitMessageT = "Waiting For Camera Calibration";
	int num_frames = 60;// Number of frames to capture
	time_t start, end;// Start and end times
	Mat frame;// Variable for storing video frames

	time(&start);// Start time
	for (int i = 0; i < num_frames; i++)// Grab a few frames
		capture.read(frame);
	time(&end);// End Time

	double seconds = difftime(end, start);// Time elapsed
	fps = (int)(num_frames / seconds);// Calculate frames per second
	movementsPerSecondRef = ceil(fps / 15);
	if (movementsPerSecondRef < 2)
		movementsPerSecondRef = 2;

	*threadWait = false;
}

//reset calibration variables
void objDet::resetCalibration()
{
	calibrationArray = { false };
	isCalibrated = false;
	pregame_origin_text.setFillColor(sf::Color::Red);
	pregame_left_text.setFillColor(sf::Color::Red);
	pregame_up_text.setFillColor(sf::Color::Red);
	pregame_down_text.setFillColor(sf::Color::Red);
	pregame_right_text.setFillColor(sf::Color::Red);
	originPoint = cv::Point(0, 0);
	originRect = cv::Rect(0, 0, 0, 0);
}