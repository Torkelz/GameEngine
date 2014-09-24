#include "HighPerformanceTimer.h"
#include <vector>
#include <algorithm>


HighPerformanceTimer::HighPerformanceTimer()
	: m_StartingTime(), m_EndingTime(), 
	m_Frequency(), m_Overhead()
{
	QueryPerformanceFrequency(&m_Frequency);
	LARGE_INTEGER start, stop;
	QueryPerformanceCounter(&start);
	QueryPerformanceCounter(&stop);
	m_Overhead.QuadPart = stop.QuadPart - start.QuadPart;
}


HighPerformanceTimer::~HighPerformanceTimer()
{
}

double HighPerformanceTimer::measureFunction(std::function<void(void)> p_Function, unsigned int p_Repeats /*= 1*/)
{
	std::vector<LONGLONG> results(p_Repeats, {});

	for (unsigned int i = 0; i < p_Repeats; ++i)
	{
		start();
		p_Function();
		results[i] = stop();
	}

	std::sort(results.begin(), results.end());
	results.erase(results.begin(), results.begin() + 10); //Remove first 10 results
	results.erase(results.end() - 10, results.end()); //Remove last 10 results
	double total = {};
	for each(const LONGLONG &r in results)
		total += r;
	total /= results.size();

	return ticksToMs(total);
}

void HighPerformanceTimer::start()
{
	m_StartingTime = {};
	QueryPerformanceCounter(&m_StartingTime);
}

LONGLONG HighPerformanceTimer::stop()
{
	QueryPerformanceCounter(&m_EndingTime);

	//Returns the elapsed amount of ticks without overhead.
	return m_EndingTime.QuadPart - m_StartingTime.QuadPart - m_Overhead.QuadPart;
}

double HighPerformanceTimer::ticksToMs(double p_Ticks)
{
	//
	// We now have the elapsed number of ticks, along with the
	// number of ticks-per-second. We use these values
	// to convert to the number of elapsed microseconds.
	// To guard against loss-of-precision, we convert
	// to microseconds *before* dividing by ticks-per-second.
	//
	p_Ticks *= 1000000;
	return p_Ticks / m_Frequency.QuadPart;
}
