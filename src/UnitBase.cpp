#include "UnitBase.h"

UnitBase::UnitBase( StateLevel* newParent )
{
	x = 0;
	y = 0;
	w = 1;
	h = 1;
	parent = newParent;
	toBeRemoved = false;
	deadlyOnTouch = false;
	invincible = false;
	#ifdef _DEBUG
	debugString = "";
	#endif
}

UnitBase::~UnitBase()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int UnitBase::update( Uint32 delta )
{
	spUpdateSprite( activeSprite, delta );
	return 0;
}

void UnitBase::render( SDL_Surface *target )
{
	spDrawSprite( x, y, -1, activeSprite );
}

void UnitBase::ai( UnitBase* player )
{
	//
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
