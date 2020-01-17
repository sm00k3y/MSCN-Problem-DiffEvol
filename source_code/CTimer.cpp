#include "CTimer.h"

CTimer::CTimer()
{
	QueryPerformanceFrequency(&li_freq);
}

void CTimer::vStart()
{
	QueryPerformanceCounter(&li_start);
}

double CTimer::dCurrentTime()
{
	QueryPerformanceCounter(&li_end);
	return (li_end.QuadPart - li_start.QuadPart) / li_freq.QuadPart;
}

void CTimer::vStop()
{
	QueryPerformanceCounter(&li_end);
	d_secs = (li_end.QuadPart - li_start.QuadPart) / li_freq.QuadPart;
}