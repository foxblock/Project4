#include "UnitBase.h"

#include "ShapeBase.h"

UnitBase::UnitBase( StateLevel* newParent, ShapeBase *newShape )
{
	parent = newParent;
	shape = newShape;
	toBeRemoved = false;
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
	*x += vel.x;
	*y += vel.y;
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
	if (props.hasFlag(ufDeadlyOnTouch) && !other->props.hasFlag(ufInvincible) )
		other->toBeRemoved = true;
}

void UnitBase::ai( Uint32 delta, UnitBase* player )
{
	//
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
