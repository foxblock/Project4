#include "UnitBase.h"

#include "ShapeBase.h"
#include "UtilityFunctions.h"
#include "gameDefines.h"

#ifdef _DEBUG
#define DEBUG_VELOCITY_LINE 200.0f
#define DEBUG_ACCELERATION_LINE 20000.0f
#endif

#ifdef _DEBUG
spFontPointer UnitBase::debugFont = NULL;
#endif

UnitBase::UnitBase( StateLevel *newParent, ShapeBase *newShape )
{
	parent = newParent;
	shape = newShape;
	toBeRemoved = false;
	x = NULL;
	y = NULL;
	activeSprite = NULL;
	maxVel = UNIT_DEFAULT_MAX_VEL;
	maxAccel = UNIT_DEFAULT_MAX_ACCEL;
	friction = UNIT_DEFAULT_FRICTION;

#ifdef _DEBUG
	debugString = "";
	if ( !debugFont )
	{
		debugFont = spFontLoad( GAME_FONT, 10 );
		spFontAddRange( debugFont, ' ', '~', spGetRGB( 255, 0, 0 ) );
	}
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
	if ( vel.lengthSquared() > Utility::sqr( friction ) * delta * delta )
		vel -= vel.unit() * friction * delta;
	else
		vel = Vector2d<float>( 0, 0 );
	if ( vel.length() > maxVel )
		vel = vel.unit() * maxVel;
	if ( accel.length() > maxAccel )
		accel = accel.unit() * maxAccel;
	*x += vel.x * delta;
	*y += vel.y * delta;
	return 0;
}

void UnitBase::render( SDL_Surface *target )
{
	if ( activeSprite )
		spDrawSprite( *x, *y, -1, activeSprite );
#ifdef _DEBUG
	shape->render( target, spGetRGB( 228, 0, 228 ) );
	spLine( *x, *y, -1, *x + vel.x * DEBUG_VELOCITY_LINE,
			*y + vel.y * DEBUG_VELOCITY_LINE, -1, spGetRGB( 0, 255, 0 ) );
	spLine( *x, *y, -1, *x + accel.x * DEBUG_ACCELERATION_LINE,
			*y + accel.y * DEBUG_ACCELERATION_LINE, -1, spGetRGB( 0, 0, 255 ) );
	spEllipseBorder( *x, *y, -1, friction / 2.0f * DEBUG_ACCELERATION_LINE,
					friction / 2.0f * DEBUG_ACCELERATION_LINE, 1, 1, spGetRGB( 0, 255, 255 ) );
	spFontDraw( *x, *y, -1, Utility::vecToStr( vel ).c_str(), debugFont );
	spFontDraw( *x, *y + 12, -1, Utility::vecToStr( accel ).c_str(), debugFont );
#endif
}

bool UnitBase::checkCollision( UnitBase const *const other ) const
{
	if ( shape->checkCollision( other->shape ) )
		return true;
	return false;
}

void UnitBase::collisionResponse( UnitBase *const other )
{
	if ( props.hasFlag( ufDeadlyOnTouch ) && !other->props.hasFlag( ufInvincible ) )
		other->toBeRemoved = true;
}

void UnitBase::ai( Uint32 delta, UnitBase *player )
{
	//
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
