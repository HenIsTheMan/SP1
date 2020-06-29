#include "timer.h"

CStopWatch::CStopWatch(){ //Constructor to set timer intervals ++requests a minimum resolution for periodic timers
	QueryPerformanceFrequency(&m_liFrequency);
	TIMECAPS tc;
	if(timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR){
		//Error, application can't continue.
	}
	m_uTimerRes = min(max(tc.wPeriodMin, 1), tc.wPeriodMax); //1 is 1 ms target resolution
	timeBeginPeriod(m_uTimerRes);
}

CStopWatch::~CStopWatch(){ //Destructor to clear the resolution
	timeEndPeriod(m_uTimerRes);
}

double CStopWatch::LiToSecs(LARGE_INTEGER & liInput){ //Convert to s (input: long int)
	return (double)liInput.QuadPart / (double)m_liFrequency.QuadPart;
}

void CStopWatch::startTimer(){ //Start the timer by getting then storing current time
	QueryPerformanceCounter(&m_liPrevTime);
}

double CStopWatch::getElapsedTime(){
	LARGE_INTEGER llTime;
	QueryPerformanceCounter(&m_liCurrTime); //Get current time
	llTime.QuadPart = m_liCurrTime.QuadPart - m_liPrevTime.QuadPart; //Elapsed time = current time - previously stored time
	m_liPrevTime = m_liCurrTime; //Store current time for the next elapsed time calculation
	return LiToSecs(llTime);
}

void CStopWatch::waitUntil(long long llTime){ //Sleep the system until llTime
	LARGE_INTEGER liCurTime;
	LONGLONG llTimeElapsed;
	while(1){
		QueryPerformanceCounter(&liCurTime);
		llTimeElapsed = ((liCurTime.QuadPart - m_liPrevTime.QuadPart) * 1000 / m_liFrequency.QuadPart);
		if(llTimeElapsed > llTime){ //End sleep if llTime has passed
			break;
		} else if(llTime - llTimeElapsed > 1){
			Sleep(1);
		}
	}
}