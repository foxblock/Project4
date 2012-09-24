#include "SpawnBase.h"

SpawnBase::SpawnBase( StateLevel *newParent )
{
	parent = newParent;
}

SpawnBase::~SpawnBase()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int SpawnBase::update( Uint32 delta )
{
	for (std::vector< Timer* >::iterator I = timers.begin(); I != timers.end(); ++I)
		(*I)->update( delta );
	return 0;
}

void SpawnBase::handleEvent( EventBase const*const event )
{
	//
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
