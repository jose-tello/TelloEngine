#ifndef __TIMER_H__
#define __TIMER_H__

struct Timer
{
public:
	Timer();

	void Start();
	void Stop();

	unsigned __int32 Read();

private:

	bool	stopped;
	unsigned __int32	startedAt;
	unsigned __int32	stoppedAt;
};

#endif //__TIMER_H__