#include "ScoreBase.h"

ScoreBase::ScoreBase( StateLevel *newParent )
{
	parent = newParent;
}

ScoreBase::~ScoreBase()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------



///--- PROTECTED ---------------------------------------------------------------

int ScoreBase::update( Uint32 delta )
{
	for (std::vector< Timer* >::iterator I = timers.begin(); I != timers.end(); ++I)
		(*I)->update( delta );
	return 0;
}

///--- PRIVATE -----------------------------------------------------------------
