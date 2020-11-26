// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL.h"

// ---------------------------------------------
Timer::Timer() : 
	stopped(true), 
	startedAt(0), 
	stoppedAt(0)
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	stopped = false;
	startedAt = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	stopped = true;
	stoppedAt = SDL_GetTicks();
}

// ---------------------------------------------
unsigned __int32 Timer::Read()
{
	if(stopped == false)
		return SDL_GetTicks() - startedAt;
	
	else
		return stoppedAt - startedAt;
}