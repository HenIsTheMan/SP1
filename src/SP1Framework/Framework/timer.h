#pragma once
#include <windows.h>

class CStopWatch{ //Do not declare public class vars, use get and set functions if you need to access class vars
	private:
		UINT m_uTimerRes;
		LARGE_INTEGER m_liPrevTime, m_liCurrTime;
		LARGE_INTEGER m_liFrequency;
		double LiToSecs(LARGE_INTEGER &liInput);
	public:
		CStopWatch(); //Constructor
		~CStopWatch(); //Destructor
		void startTimer(); //Start the timer by getting then storing current time
		double getElapsedTime(); //Get time in s since the last call to this function
		void waitUntil(long long llTime); //Wait until a time in ms has passed
};