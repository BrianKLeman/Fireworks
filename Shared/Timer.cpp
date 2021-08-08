#include "Timer.h"

Timer::Timer()
{	
	  QueryPerformanceFrequency(&ticksPerSecond);
	  QueryPerformanceCounter(&start_time);
}

Timer::~Timer(void)
{
}

float Timer::TotalElapsedSeconds()
{
	LARGE_INTEGER now;
	// get time
	QueryPerformanceCounter(&now);
	now.QuadPart -= start_time.QuadPart;

	// convert the tick number into the number of seconds
	// since the system was started...
	float seconds = (float)(now.QuadPart/ticksPerSecond.QuadPart);  
	float fraction = ((float)(now.QuadPart % ticksPerSecond.QuadPart) / (float)ticksPerSecond.QuadPart);

	return seconds+fraction;
}

void Timer::Reset(void)
{
	QueryPerformanceCounter(&start_time);
}
