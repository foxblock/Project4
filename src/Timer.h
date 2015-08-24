#ifndef _TIMER_H
#define _TIMER_H

class Timer
{
public:
	Timer();
	virtual ~Timer();

	// pass a value >0 for a countdown and <= for stopwatch
	void start( const int &milliseconds=0 );
	void pause();
	// this also resets the mode to -1
	void stop();

	void update( const int &delta );

	// time remaining on countdown, time running on stopwatch
	int getTime() const;
	// limit the countdown was started with, 0 on stopwatch
	int getDuration() const { return duration; }
	// -1 - stopped, 0 - paused, 1 - running
	int getStatus() const { return status; }
	// -1 - off, 0 - stopwatch, 1 - countdown
	int getMode() const { return mode; }

	// returns true as soon as the timer was started and even when it has run out
	// returns false after creation and when the timer was manually stopped
	bool started() const { return (mode != -1); }
	// returns true after creation, when a count-down ran out or the Timer is manually stopped
	// returns false while the timer is running
	bool stopped() const { return (status == -1); }
	// returns true when the countdown has run out after it was started
	// (when wasStarted and isStopped return true)
	bool finished() const { return started() && stopped(); }
	// returns true while the timer is running
	bool running() const { return started() && !stopped(); }

protected:
	int duration;
	int currentTicks;

	int status;
	int mode;
private:

};


#endif // _TIMER_H

