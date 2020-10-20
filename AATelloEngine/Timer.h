#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"


class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	unsigned __int32 Read();

private:

	bool	running;
	unsigned __int32	startedAt;
	unsigned __int32	stoppedAt;
};

#endif //__TIMER_H__