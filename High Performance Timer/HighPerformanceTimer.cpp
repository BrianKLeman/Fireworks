#include "HighPerformanceTimer.h"


CHighPerformanceTimer::CHighPerformanceTimer(int id)
{
	// get the high resolution counter's accuracy
	id_=1;
	  QueryPerformanceFrequency(&ticksPerSecond);
	  QueryPerformanceCounter(&start_time);
}


CHighPerformanceTimer::~CHighPerformanceTimer(void)
{
}

float CHighPerformanceTimer::getTimeElapsedFromStart()
{
	LARGE_INTEGER now;
	// get time
  	QueryPerformanceCounter(&now);

	now.QuadPart -= start_time.QuadPart;

  // convert the tick number into the number of seconds
  // since the system was started...
	float seconds = (float)(now.QuadPart/ticksPerSecond.QuadPart);

  ////get the number of hours
  //int hours = time.QuadPart/3600;

  ////get the number of minutes
  //time.QuadPart = time.QuadPart - (hours * 3600);
  //int minutes = time.QuadPart/60;

  ////get the number of seconds
  //int seconds = time.QuadPart - (minutes * 60);

  //gives the fractions of a second left after the nearest elapsed second
  
  float fraction = ((float)(now.QuadPart % ticksPerSecond.QuadPart) / (float)ticksPerSecond.QuadPart);

  return seconds+fraction;

}
// reset the timer
void CHighPerformanceTimer::reset(void)
{
	QueryPerformanceCounter(&start_time);
}
