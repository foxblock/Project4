#include "StateBase.h"

StateBase::StateBase()
{
	errorString = "";
	type = stNone;
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

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
