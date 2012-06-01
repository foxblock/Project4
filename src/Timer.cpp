#include "Timer.h"

#include "SDL/SDL.h"

Timer::Timer()
{
	startTicks = 0;
	pauseTicks = 0;
	duration = 0;
	status = -1;
	mode = -1;
}

Timer::~Timer()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

void Timer::start( int milliseconds )
{
	startTicks = SDL_GetTicks();
	status = 1;

	duration = milliseconds;
	if ( duration <= 0 )
		mode = 0;
	else
		mode = 1;
}

void Timer::pause()
{
	switch ( status )
	{
	case -1:
		return;
	case 0:
		startTicks += SDL_GetTicks() - pauseTicks;
		status = 1;
		break;
	case 1:
		pauseTicks = SDL_GetTicks();
		status = 0;
		break;
	}
}

void Timer::stop()
{
	status = -1;
	mode = -1;
}

int Timer::getTime()
{
	int temp = 0;

	switch ( status )
	{
	case -1:
		return 0;
	case 0:
		temp = pauseTicks;
		break;
	case 1:
		temp = SDL_GetTicks();
		break;
	}
	if ( mode == 0 )
		return temp - startTicks;
	else
	{
		int remaining = duration - ( temp - startTicks );
		if ( remaining < 0 )
		{
			status = -1;
			return 0;
		}
		return remaining;
	}
}

int Timer::getStatus()
{
	if ( mode == 1 )
	{
		int temp = 0;
		switch ( status )
		{
		case -1:
			return -1;
		case 0:
			temp = pauseTicks;
			break;
		case 1:
			temp = SDL_GetTicks();
			break;
		}
		int remaining = duration - ( temp - startTicks );
		if ( remaining < 0 )
		{
			status = -1;
		}
	}
	return status;
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
