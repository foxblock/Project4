#include "UnitBase.h"

#include "ShapeBase.h"

UnitBase::UnitBase( StateLevel* newParent, ShapeBase *newShape )
{
	parent = newParent;
	toBeRemoved = false;
	deadlyOnTouch = false;
	invincible = false;
	shape = newShape;
	x = NULL;
	y = NULL;
	activeSprite = NULL;
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
	if ( activeSprite )
		spUpdateSprite( activeSprite, delta );
	return 0;
}

void UnitBase::render( SDL_Surface *target )
{
	if ( activeSprite )
		spDrawSprite( *x, *y, -1, activeSprite );
}

bool UnitBase::checkCollision( UnitBase const * const other ) const
{
	if ( shape->checkCollision( other->shape ) )
		return true;
	return false;
}

void UnitBase::collisionResponse( UnitBase * const other )
{
	if (deadlyOnTouch && !other->invincible )
		other->toBeRemoved = true;
}

void UnitBase::ai( UnitBase* player )
{
	//
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
