#ifndef _TIMER_H
#define _TIMER_H

class Timer
{
public:
	Timer();
	virtual ~Timer();

	// pass a value >0 for a countdown and <= for stopwatch
	void start( int milliseconds=0 );
	void pause();
	void stop();

	void update( int delta );

	// time remaining on countdown, time running on stopwatch
	int getTime();
	// -1 - stopped, 0 - paused, 1 - running
	int getStatus() { return status; }
	// -1 - off, 0 - stopwatch, 1 - countdown
	int getMode() { return mode; }

	bool wasStarted() { return (mode != -1); }
	bool isStopped() { return (status == -1); }

protected:
	int duration;
	int currentTicks;

	int status;
	int mode;
private:

};


#endif // _TIMER_H

