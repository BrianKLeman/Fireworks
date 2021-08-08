#pragma once
//written using code from http://www.decompile.com/cpp/faq/windows_timer_api.htm
#include <windows.h>

class Timer
{
public:
	Timer();
	~Timer(void);
	float TotalElapsedSeconds();
	void Reset(void);

private:
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER tick;   // A point in time
	LARGE_INTEGER time;   // For converting tick into real time
	LARGE_INTEGER start_time;   // time of construction
};