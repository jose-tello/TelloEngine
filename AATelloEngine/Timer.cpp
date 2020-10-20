// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL.h"

// ---------------------------------------------
Timer::Timer() : 
	running(false), 
	startedAt(0), 
	stoppedAt(0)
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	startedAt = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stoppedAt = SDL_GetTicks();
}

// ---------------------------------------------
unsigned __int32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - startedAt;
	}
	else
	{
		return stoppedAt - startedAt;
	}
}