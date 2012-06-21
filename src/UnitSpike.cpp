#include "UnitSpike.h"

#include <cmath>
#include "UtilityFunctions.h"
#include "gameDefines.h"

// Pixels per millisecond
#define SPIKE_ATTACK_ACCEL 0.008f
#define SPIKE_CHARGE_SPEED 0.75f
#define SPIKE_MOVEMENT_SPEED 0.5f
#define SPIKE_IDLE_SPEED 0.1f
#define SPIKE_IDLE_FRICTION 0.001f
#define SPIKE_IDLE_ACCEL 0.005f
#define SPIKE_ATTACK_RADIUS_SQR 40000.0f
#define SPIKE_CHARGE_RADIUS_SQR 5625.0f
#define SPIKE_WAIT_TIME 500
#define SPIKE_CHARGE_TIME 500

UnitSpike::UnitSpike( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	idle = spNewSprite();
	attack = spNewSprite();
	image = spLoadSurface( "images/units/spike.png" );
	spNewSubSpriteWithTiling( idle, image, 0, 0, 64, 64, 1000 );
	spNewSubSpriteWithTiling( attack, image, 64, 0, 64, 64, 1000 );
	activeSprite = idle;
	shape.radius = 20;
	x = &( shape.pos.x );
	y = &( shape.pos.y );
	chargeState = 0;
	maxVel = SPIKE_IDLE_SPEED;
	maxAccel = SPIKE_IDLE_ACCEL;
	friction = SPIKE_IDLE_FRICTION;
}

UnitSpike::~UnitSpike()
{
	spDeleteSprite( idle );
	spDeleteSprite( attack );
	SDL_FreeSurface( image );
}


///--- PUBLIC ------------------------------------------------------------------

void UnitSpike::ai( Uint32 delta, UnitBase *player )
{
	Vector2d<float> diff( *player->x - *x, *player->y - *y );
	float dist = diff.lengthSquared();
	if ( chargeTimer.getStatus() == -1 && chargeState == 1 )
	{
		maxVel = UNIT_DEFAULT_MAX_VEL;
		vel = diff.unit() * SPIKE_CHARGE_SPEED;
		friction = 0;
		activeSprite = attack;
		props.addFlag( ufDeadlyOnTouch );
		chargeState = 2;
		chargeTimer.start( SPIKE_CHARGE_TIME );
	}
	if ( chargeTimer.getStatus() != -1 && chargeState == 2 &&
		!shape.pos.isInRect(Vector2d<float>(0,0),Vector2d<float>(APP_SCREEN_WIDTH,APP_SCREEN_WIDTH)) )
	{
		chargeTimer.stop();
	}
	if ( chargeTimer.getStatus() == -1 && chargeState == 2 )
	{
		chargeState = 0;
		activeSprite = idle;
		props.removeFlag( ufDeadlyOnTouch );
		vel = Vector2d<float>( 0, 0 );
		accel = Vector2d<float>( 0, 0 );
	}
	if ( chargeState == 0 )
	{
		if ( dist < SPIKE_CHARGE_RADIUS_SQR )
		{
			chargeTimer.start( SPIKE_WAIT_TIME );
			chargeState = 1;
			vel = Vector2d<float>( 0, 0 );
			accel = Vector2d<float>( 0, 0 );
		}
		else if ( dist < SPIKE_ATTACK_RADIUS_SQR )
		{
			maxVel = SPIKE_MOVEMENT_SPEED;
			friction = UNIT_DEFAULT_FRICTION;
			maxAccel = UNIT_DEFAULT_MAX_ACCEL;
			accel = diff.unit() * SPIKE_ATTACK_ACCEL;
		}
		else
		{
			maxVel = SPIKE_IDLE_SPEED;
			maxAccel = SPIKE_IDLE_ACCEL;
			friction = SPIKE_IDLE_FRICTION;
			Vector2d<float> temp(rand() % 3 - 1, rand() % 3 - 1);
			accel += temp.unit() / 5000.0f;
			if ( (*x < shape.radius && accel.x < 0) ||
					(*x > APP_SCREEN_WIDTH - shape.radius && accel.x > 0 ) )
				accel.x *= -1;
			if ( ( *y < shape.radius && accel.y < 0 ) ||
					(*y > APP_SCREEN_HEIGHT - shape.radius && accel.y > 0 ) )
				accel.y *= -1;
		}
	}
}

void UnitSpike::render( SDL_Surface *target )
{
	UnitBase::render( target );
	if ( chargeState == 1 )
	{
		spEllipse( *x, *y, -1, 20 * ( SPIKE_WAIT_TIME - chargeTimer.getTime() ) / SPIKE_WAIT_TIME,
				   20 * ( SPIKE_WAIT_TIME - chargeTimer.getTime() ) / SPIKE_WAIT_TIME,
				   spGetRGB( 255, 0 , 0 ) );
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
