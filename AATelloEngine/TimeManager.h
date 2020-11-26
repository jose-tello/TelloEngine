#ifndef __TIME_MAGER_H__
#define __TIME_MAGER_H__

#include "Timer.h"

struct TimeManager
{
public:
	TimeManager();

	void Update();

	float GetTimeSinceStart() const;
	float GetTimeSincePlay() const;
	float GetDt();

	void StartPlayTimer();
	void ContinuePlayTimer();
	void PausePlayTimer();
	void StopPlayTimer();
	void StepFrame();

	bool IsGameStarted();

private:
	Timer timer;

	unsigned __int32 ticksSinceStart;
	unsigned __int32 ticksSincePlay;
	unsigned __int32 dt;

	bool playStopped;
	bool advanceFrame;
};

#endif // !__TIME_MAGER_H__
