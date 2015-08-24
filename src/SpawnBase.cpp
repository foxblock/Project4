#include "SpawnBase.h"

// Unit classes
#include "UnitPlayer.h"
#include "UnitSpike.h"
#include "UnitLaser.h"
#include "UnitBomb.h"
#include "UnitText.h"
#include "ItemSlowmo.h"
#include "ItemVortex.h"

#include "gameDefines.h"

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

void SpawnBase::render(SDL_Surface* target)
{
	//
}

UnitBase* SpawnBase::getUnit(const int& type) const
{
	UnitBase *unit = NULL;
	switch ( type )
	{
	case UnitBase::utSpike:
		unit = new UnitSpike( parent );
		break;
	case UnitBase::utLaser:
		unit = new UnitLaser( parent );
		break;
	case UnitBase::utBomb:
		unit = new UnitBomb( parent );
		break;
	case UnitBase::utText:
		unit = new UnitText( parent );
		break;

	case UnitBase::utItemSlowmo:
		unit = new ItemSlowmo( parent );
		break;
	case UnitBase::utItemVortex:
		unit = new ItemVortex( parent );
		break;
	default:
		printf( "%s Passed type to SpawnBase::getUnit is invalid: %i\n", ERROR_STRING, type );
		break;
	}

	return unit;
}


///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
