#include "pch.h"


void errorMessageExit(string message) {//exits program and displays error message

	MessageBox(NULL, wstring(message.begin(), message.end()).c_str(), L"ERROR", MB_SETFOREGROUND);
	exit(EXIT_FAILURE);
}

void message(string message, string title) {//displays message 

	MessageBox(NULL, wstring(message.begin(), message.end()).c_str(), wstring(title.begin(), title.end()).c_str(), MB_SETFOREGROUND);
}