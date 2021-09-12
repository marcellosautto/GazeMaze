#pragma once

//Define
#define PROG_NAME "Gazer_Eyes"
#define NOMINMAX

//OpenCV Lib
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/imgcodecs.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/video.hpp>
#include "opencv2/videoio.hpp"
#include <opencv2/core/cvstd.hpp>

//SFML Lib
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

//C++ Lib
#include <iostream>
#include <array>
#include <Windows.h>
#include <string>
#include <fstream>
#include <random>
#include <stack>
#include <stdio.h>
#include <sstream>
#include <unordered_map>
#include <map>
#include <conio.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include <ratio>
#include <ctime>
#include <Lmcons.h>

using namespace std;
using namespace std::chrono;

//Global Commands
#ifndef GUARD_H
#define GUARD_H

void errorMessageExit(string message);
void message(string message, string title);

#endif 
