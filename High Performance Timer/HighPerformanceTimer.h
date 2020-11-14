#pragma once
//written using code from http://www.decompile.com/cpp/faq/windows_timer_api.htm
#include <windows.h>

class CHighPerformanceTimer
{
public:
CHighPerformanceTimer(int);
~CHighPerformanceTimer(void);
int id_;
float getTimeElapsedFromStart();
LARGE_INTEGER ticksPerSecond;
LARGE_INTEGER tick;   // A point in time
LARGE_INTEGER time;   // For converting tick into real time

LARGE_INTEGER start_time;   // time of construction

// reset the timer
void reset(void);
};

extern CHighPerformanceTimer* timer;