#include "StateBase.h"

StateBase::StateBase()
{
	errorString = "";
	message = "";
	type = stNone;
	paused = false;
}

StateBase::~StateBase()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int StateBase::update(Uint32 delta)
{
	for ( std::vector< Timer* >::iterator I = timers.begin(); I != timers.end(); ++I )
		(*I)->update( delta );
	return 0;
}

int StateBase::pauseUpdate(Uint32 delta)
{
	return 0;
}

void StateBase::pauseRender(SDL_Surface* target)
{
	render( target );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
