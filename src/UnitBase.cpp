#include "UnitBase.h"

#include "ShapeBase.h"
#include "UtilityFunctions.h"

UnitBase::UnitBase( StateLevel* newParent, ShapeBase *newShape )
{
	parent = newParent;
	shape = newShape;
	toBeRemoved = false;
	x = NULL;
	y = NULL;
	activeSprite = NULL;
	maxMovementSpeed = PHYSICS_DEFAULT_MAXIMUM;
	friction = PHYSICS_DEFAULT_FRICTION;

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

	vel += accel * delta;
	if ( vel.lengthSquared() > Utility::sqr(friction) * delta )
		vel -= vel.unit() * friction;
	else
		vel = Vector2d<float>(0,0);
	if ( vel.length() > maxMovementSpeed )
		vel = vel.unit() * maxMovementSpeed;
	*x += vel.x * delta;
	*y += vel.y * delta;
#ifdef _DEBUG
	debugString = "V: " + Utility::vecToStr( vel ) + " \n A: " + Utility::vecToStr( accel );
#endif
	return 0;
}

void UnitBase::render( SDL_Surface *target )
{
	if ( activeSprite )
		spDrawSprite( *x, *y, -1, activeSprite );
#ifdef _DEBUG
	shape->render( target, SDL_MapRGB(target->format, 228, 0, 228 ) );
#endif
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
