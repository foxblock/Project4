#include "UnitBomb.h"

#include "gameDefines.h"
#include "UtilityFunctions.h"
#include "Events.h"
#include "StateLevel.h"

#include "sparrowPrimitives.h"

#define BOMB_RADIUS 6
#define BOMB_PRESSURE_RADIUS_SQR_HI  22500.0f
#define BOMB_PRESSURE_RADIUS_SQR_MID 10000.0f
#define BOMB_PRESSURE_RADIUS_SQR_LOW  3600.0f
// pressure levels in header
#define BOMB_PRESSURE_TIMER_1 200
#define BOMB_PRESSURE_TIMER_2 120
#define BOMB_PRESSURE_TIMER_3 50
#define BOMB_PRESSURE_ADD_1 1
#define BOMB_PRESSURE_ADD_2 2
#define BOMB_PRESSURE_ADD_3 3
#define BOMB_PRESSURE_REL 1
#define BOMB_EVASION_RADIUS_SQR 90000.0f
#define BOMB_IDLE_MAX_ACCEL 0.005f
#define BOMB_IDLE_ACCEL 0.00005f
#define BOMB_IDLE_MAX_VEL 0.07f
#define BOMB_IDLE_FRICTION 0.001f
#define BOMB_EVASION_ACCEL 0.002f
#define BOMB_EVASION_MAX_VEL 0.2f
#define BOMB_MIN_EVADE_TIME 250
#define BOMB_EXPLOSION_TIME 500
#define BOMB_EXPLOSION_RADIUS 100

SDL_Surface* UnitBomb::idle = NULL;
SDL_Surface* UnitBomb::flashing = NULL;

UnitBomb::UnitBomb( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	if ( !idle )
		generateIdleImage();
	if ( !flashing )
		generateFlashingImage();
	activeSprite = idle;

	shape.radius = BOMB_RADIUS;

	pressure = 0;
	status = 0;
	maxVel = BOMB_IDLE_MAX_VEL;
	maxAccel = BOMB_IDLE_MAX_ACCEL;
	friction = BOMB_IDLE_FRICTION;
	isFlashing = false;
	type = utBomb;
	timers.push_back( &bombTimer );
	timers.push_back( &flashTimer );
	timers.push_back( &evadeTimer );
}

UnitBomb::~UnitBomb()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int UnitBomb::update( const Uint32 &delta )
{
	if ( isFlashing )
		activeSprite = flashing;
	else
		activeSprite = idle;

	if ( !bombTimer.stopped() )
	{
		float radius = BOMB_EXPLOSION_RADIUS * (float)(BOMB_EXPLOSION_TIME - bombTimer.getTime()) / (float)BOMB_EXPLOSION_TIME;
		shape.radius = radius;
		activeSprite = NULL;
	}

	if ( bombTimer.finished() )
	{
		LOG_MESSAGE("Removing exploded bomb");
		toBeRemoved = true;
		EventUnitDeath *event = new EventUnitDeath( this, NULL );
		parent->addEvent( event );
	}

	return UnitBase::update( delta );
}

void UnitBomb::render( SDL_Surface *const target )
{
#ifdef _DEBUG
	debugString += Utility::numToStr( pressure ) + "\n" + Utility::numToStr( status ) + "\n";
#endif
	if ( !bombTimer.stopped() )
	{
		spEllipse( *x, *y, -1, shape.radius, shape.radius, spGetFastRGB( 255, 0, 0 ) );
	}
	UnitBase::render( target );
}

void UnitBomb::collisionResponse( UnitBase *const other )
{
	if ( other->type == utBomb )
	{
		if ( flags.has( ufDeadlyOnTouch ) && !other->flags.has( ufInvincible ) )
		{
			((UnitBomb*)other)->pressure = BOMB_PRESSURE_LEVEL_5;
			EventBombCascade *event = new EventBombCascade( this, other );
			parent->addEvent( event );
		}
	}
	else
		UnitBase::collisionResponse( other );
}

void UnitBomb::ai( const Uint32 &delta, UnitBase *const player )
{
	if ( bombTimer.started() )
		return;

	Vector2d<float> diff( *player->x - *x, *player->y - *y );
	float dist = diff.lengthSquared();

	// player close -> +pressure
	if ( dist < BOMB_PRESSURE_RADIUS_SQR_HI )
	{
		pressure += BOMB_PRESSURE_ADD_1 * delta;
		maxVel = BOMB_EVASION_MAX_VEL;
	}
	else if ( dist < BOMB_PRESSURE_RADIUS_SQR_MID )
		pressure += BOMB_PRESSURE_ADD_2 * delta;
	else if ( dist < BOMB_PRESSURE_RADIUS_SQR_LOW )
		pressure += BOMB_PRESSURE_ADD_3 * delta;
	else if ( pressure > 0 )
		pressure -= BOMB_PRESSURE_REL * delta;

	bool idleRoaming = true;
	// evasion
	if ( dist < BOMB_EVASION_RADIUS_SQR )
	{
		Vector2d<float> playerAcc = player->accel.unit();
		Vector2d<float> distUnit = -diff.unit();
		if ( dist < BOMB_PRESSURE_RADIUS_SQR_HI ||
			((playerAcc.x != 0 || playerAcc.y != 0) &&
			fabs(playerAcc.x - distUnit.x) < 0.5f &&
			fabs(playerAcc.y - distUnit.y) < 0.5f) )
		{
			accel = distUnit * BOMB_EVASION_ACCEL;
			maxVel = BOMB_EVASION_MAX_VEL;
			status = 1;
			idleRoaming = false;
		}
	}
	// min evade - when exiting evasion radius (to prevent immediate re-enter)
	if ( idleRoaming && status == 1 )
	{
		idleRoaming = false;
		status = 2;
		evadeTimer.start( BOMB_MIN_EVADE_TIME );
	}
	if ( evadeTimer.getStatus() == 1 )
		idleRoaming = false;

	// idle movement
	if ( idleRoaming )
	{
		// reset evasion status
		if ( status > 0 )
		{
			status = 0;
			accel = Vector2d<float>(0,0);
		}
		maxVel = BOMB_IDLE_MAX_VEL;
		Vector2d<float> temp( Utility::randomRange( -1, 1 ), Utility::randomRange( -1, 1 ) );
		accel += temp.unit() * BOMB_IDLE_ACCEL * delta;
		if ( ( *x < shape.radius && accel.x < 0 ) ||
		( *x > APP_SCREEN_WIDTH - shape.radius && accel.x > 0 ) )
			accel.x *= -1;
		if ( ( *y < shape.radius && accel.y < 0 ) ||
		( *y > APP_SCREEN_HEIGHT - shape.radius && accel.y > 0 ) )
			accel.y *= -1;
	}

	// pressure levels - flashing and explosion
	if ( pressure > BOMB_PRESSURE_LEVEL_4 )
	{
		bombTimer.start( BOMB_EXPLOSION_TIME );
		accel = Vector2d<float>(0,0);
		vel = Vector2d<float>(0,0);
		flags.add( ufDeadlyOnTouch );
		flags.add( ufInvincible );
		LOG_MESSAGE("Bomb exploding");
	}
	else if ( pressure > BOMB_PRESSURE_LEVEL_3 )
	{
		if ( flashTimer.stopped() )
		{
			flashTimer.start( BOMB_PRESSURE_TIMER_3 );
			isFlashing = !isFlashing;
		}
	}
	else if ( pressure > BOMB_PRESSURE_LEVEL_2 )
	{
		if ( flashTimer.stopped() )
		{
			flashTimer.start( BOMB_PRESSURE_TIMER_2 );
			isFlashing = !isFlashing;
		}
	}
	else if ( pressure > BOMB_PRESSURE_LEVEL_1 )
	{
		if ( flashTimer.stopped() )
		{
			flashTimer.start( BOMB_PRESSURE_TIMER_1 );
			isFlashing = !isFlashing;
		}
	}
	else
	{
		flashTimer.stop();
		isFlashing = false;
	}

	if ( *x < 0 )
		*x = 0;
	else if ( *x > APP_SCREEN_WIDTH )
		*x = APP_SCREEN_WIDTH;
	if ( *y < 0 )
		*y = 0;
	else if ( *y > APP_SCREEN_HEIGHT )
		*y = APP_SCREEN_HEIGHT;
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

void UnitBomb::generateIdleImage()
{
	idle = spCreateSurface( BOMB_RADIUS * 2, BOMB_RADIUS * 2 );
	SDL_FillRect( idle, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( idle );
	spEllipse( BOMB_RADIUS ,BOMB_RADIUS, -1, BOMB_RADIUS, BOMB_RADIUS, 0 );
	spSelectRenderTarget( spGetWindowSurface() );
}

void UnitBomb::generateFlashingImage()
{
	flashing = spCreateSurface( BOMB_RADIUS * 2, BOMB_RADIUS * 2 );
	SDL_FillRect( flashing, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( flashing );
	spEllipse( BOMB_RADIUS ,BOMB_RADIUS, -1, BOMB_RADIUS, BOMB_RADIUS, spGetFastRGB( 255, 255, 255 ) );
	spSelectRenderTarget( spGetWindowSurface() );
}
