#include "UnitBase.h"

#include "ShapeBase.h"
#include "UtilityFunctions.h"
#include "gameDefines.h"
#include "Events.h"
#include "StateLevel.h"

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
	if ( shape )
	{
		x = &( shape->pos.x );
		y = &( shape->pos.y );
	}
	else
	{
		x = NULL;
		y = NULL;
	}
	activeSprite = NULL;
	maxVel = UNIT_DEFAULT_MAX_VEL;
	maxAccel = UNIT_DEFAULT_MAX_ACCEL;
	friction = UNIT_DEFAULT_FRICTION;
	type = utNone;

#ifdef _DEBUG
	debugString = "";
	if ( !debugFont )
	{
		debugFont = spFontLoad( FONT_GENERAL, 10 );
		spFontAdd( debugFont, SP_FONT_GROUP_ASCII, spGetFastRGB( 255, 0, 0 ) );
	}
#endif
}

UnitBase::~UnitBase()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int UnitBase::update( const Uint32 &delta )
{
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
	for ( std::vector< Timer * >::iterator I = timers.begin(); I != timers.end(); ++I )
		( *I )->update( delta );
	return 0;
}

void UnitBase::render( SDL_Surface *const target )
{
	if ( activeSprite )
		spBlitSurface( *x, *y, -1, activeSprite );
#ifdef _DEBUG
	debugString += Utility::vecToStr( accel ) + "\n" + Utility::vecToStr( vel ) + "\n";
	shape->render( target, spGetRGB( 228, 0, 228 ) );
	spLine( *x, *y, -1, *x + vel.x * DEBUG_VELOCITY_LINE,
			*y + vel.y * DEBUG_VELOCITY_LINE, -1, spGetFastRGB( 0, 255, 0 ) );
	spLine( *x, *y, -1, *x + accel.x * DEBUG_ACCELERATION_LINE,
			*y + accel.y * DEBUG_ACCELERATION_LINE, -1, spGetFastRGB( 0, 0, 255 ) );
	spEllipseBorder( *x, *y, -1, maxVel * DEBUG_VELOCITY_LINE,
					 maxVel * DEBUG_VELOCITY_LINE, 1, 1, spGetFastRGB( 0, 255, 0 ) );
	spEllipseBorder( *x, *y, -1, maxAccel * DEBUG_ACCELERATION_LINE,
					 maxAccel * DEBUG_ACCELERATION_LINE, 1, 1, spGetFastRGB( 0, 0, 255 ) );
	spEllipseBorder( *x, *y, -1, friction * DEBUG_ACCELERATION_LINE,
					 friction * DEBUG_ACCELERATION_LINE, 1, 1, spGetFastRGB( 0, 255, 255 ) );
	spFontDraw( *x, *y, -1, debugString.c_str(), debugFont );
	debugString = "";
#endif
}

bool UnitBase::checkCollision( UnitBase const *const other ) const
{
	if ( shape && other->shape && shape->checkCollision( other->shape ) )
		return true;
	return false;
}

void UnitBase::collisionResponse( UnitBase *const other )
{
	if ( flags.has( ufDeadlyOnTouch ) && !other->flags.has( ufInvincible ) )
	{
		LOG_MESSAGE( "Unit beeing killed" );
		other->toBeRemoved = true;
		EventUnitDeath *event = new EventUnitDeath( other, this );
		parent->addEvent( event );
	}
}

void UnitBase::ai( const Uint32 &delta, UnitBase *const player )
{
	//
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
