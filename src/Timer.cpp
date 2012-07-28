#include "Timer.h"

#include "SDL.h"

Timer::Timer()
{
	duration = 0;
	status = -1;
	mode = -1;
	currentTicks = 0;
}

Timer::~Timer()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

void Timer::start( const int &milliseconds )
{
	currentTicks = 0;
	status = 1;

	duration = milliseconds;
	if ( duration <= 0 )
	{
		mode = 0;
		duration = 0;
	}
	else
		mode = 1;
}

void Timer::pause()
{
	if ( status == 1 )
		status = 0;
	else if ( status == 0 )
		status = 1;
}

void Timer::stop()
{
	status = -1;
	mode = -1;
}

void Timer::update( const int &delta )
{
	if ( status == 1 )
		currentTicks += delta;
	if ( mode == 1 && status == 1 && currentTicks >= duration )
		status = -1;
}

int Timer::getTime() const
{
	if ( status == -1 )
		return 0;
	else
	{
		if ( mode == 0 )
			return currentTicks;
		else
			return duration - currentTicks;
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
