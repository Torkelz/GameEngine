#pragma once

//http://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <functional>

class HighPerformanceTimer
{
private:
	LARGE_INTEGER m_StartingTime, m_EndingTime;
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_Overhead;
public:
	HighPerformanceTimer();
	~HighPerformanceTimer();

	/** 
	* Measures the time it takes for the given function to finish.
	* @param p_Function The function to be measured. Must be of type void foo(void). Usage eg. std::bind(foo);
	* @param p_Repeats Default 1, sets the amount of times the function should be timed.
	* @return Returns the time in ms.
	*/
	double measureFunction(std::function<void(void)> p_Function, unsigned int p_Repeats = 1);

	void start();
	LONGLONG stop();
	double ticksToMs(LONGLONG p_Ticks);
};

