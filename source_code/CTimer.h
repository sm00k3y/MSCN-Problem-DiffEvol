#pragma once
#include <Windows.h>

class CTimer
{
public:
	CTimer();

	void vStart();
	double dCurrentTime();
	void vStop();

	double dGetTime() { return d_secs; }

private:
	double d_secs = 0.0;
	LARGE_INTEGER li_start, li_end, li_freq;
};

