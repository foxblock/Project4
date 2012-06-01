#ifndef _TIMER_H
#define _TIMER_H

class Timer
{
public:
	Timer();
	virtual ~Timer();

	// pass a value >0 for a countdown and <= for stopwatch
	void start( int milliseconds );
	void pause();
	void stop();

	// time remaining on countdown, time running on stopwatch
	int getTime();
	// -1 - stopped, 0 - paused, 1 - running
	int getStatus();
	// -1 - off, 0 - stopwatch, 1 - countdown
	int getMode() { return mode; };

protected:
	int startTicks;
	int duration;
	int pauseTicks;

	int status;
	int mode;
private:

};


#endif // _TIMER_H

