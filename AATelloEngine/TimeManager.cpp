#include "TimeManager.h"

TimeManager::TimeManager() :
	timer(),
	ticksSinceStart(0),
	ticksSincePlay(0),
	dt(0),

	playStopped(true),
	advanceFrame(false)
{
	timer.Start();
}

void TimeManager::Update()
{
	dt = timer.Read();
	ticksSinceStart += dt;
	
	if (playStopped == false)
		ticksSincePlay += dt;

	timer.Start();
}

float TimeManager::GetTimeSinceStart() const
{
	return (float)ticksSinceStart / 1000.f;
}


float TimeManager::GetTimeSincePlay() const
{
	return (float)ticksSincePlay / 1000.f;
}


float TimeManager::GetDt()
{
	if (advanceFrame == false)
	{
		if (playStopped == true)
			return 0;
	}

	advanceFrame = false;
	return (float)dt / 1000.f;
}


void TimeManager::StartPlayTimer()
{
	ticksSincePlay = 0;
	playStopped = false;
}


void TimeManager::ContinuePlayTimer()
{
	playStopped = false;
}


void TimeManager::PausePlayTimer()
{
	playStopped = true;
}


void TimeManager::StopPlayTimer()
{
	ticksSincePlay = 0;
	playStopped = true;
}


void TimeManager::StepFrame()
{
	advanceFrame = true;
}