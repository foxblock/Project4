#include "UnitSpike.h"

#include <cmath>
#include "UtilityFunctions.h"

// Pixels per millisecond
#define SPIKE_ATTACK_ACCEL 0.003f
#define SPIKE_CHARGE_SPEED 0.75f
#define SPIKE_ATTACK_RADIUS_SQR 40000.0f
#define SPIKE_CHARGE_RADIUS_SQR 5625.0f
#define SPIKE_WAIT_TIME 500
#define SPIKE_CHARGE_TIME 500
#define SPIKE_MOVEMENT_SPEED 0.5f

UnitSpike::UnitSpike( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	idle = spNewSprite();
	attack = spNewSprite();
	image = spLoadSurface( "images/units/spike.png" );
	spNewSubSpriteWithTiling( idle, image, 0, 0, 64, 64, 1000 );
	spNewSubSpriteWithTiling( attack, image, 64, 0, 64, 64, 1000 );
	activeSprite = idle;
	shape.radius = 20;
	//shape.size = Vector2d<float>(36,36);
	x = &( shape.pos.x );
	y = &( shape.pos.y );
	chargeState = 0;
	maxMovementSpeed = SPIKE_MOVEMENT_SPEED;
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
		maxMovementSpeed = PHYSICS_DEFAULT_MAXIMUM;
		vel = diff.unit() * SPIKE_CHARGE_SPEED;
		friction = 0;
		activeSprite = attack;
		props.addFlag( ufDeadlyOnTouch );
		chargeState = 2;
		chargeTimer.start( SPIKE_CHARGE_TIME );
	}
	if ( chargeTimer.getStatus() == -1 && chargeState == 2 )
	{
		maxMovementSpeed = SPIKE_MOVEMENT_SPEED;
		chargeState = 0;
		activeSprite = idle;
		props.removeFlag( ufDeadlyOnTouch );
		friction = PHYSICS_DEFAULT_FRICTION;
	}
	if ( chargeState == 0 )
	{
		if ( dist < SPIKE_CHARGE_RADIUS_SQR )
		{
			chargeTimer.start( SPIKE_WAIT_TIME );
			chargeState = 1;
			vel = Vector2d<float>(0,0);
			accel = Vector2d<float>(0,0);
		}
		else if ( dist < SPIKE_ATTACK_RADIUS_SQR )
		{
			accel = diff.unit() * SPIKE_ATTACK_ACCEL * delta;
		}
		else
		{
			vel = Vector2d<float>(0,0);
			accel = Vector2d<float>(0,0);
		}
	}
}

void UnitSpike::render( SDL_Surface *target )
{
	UnitBase::render( target );
	if ( chargeState == 1 )
	{
		spEllipse( *x, *y, -1, 20 * (SPIKE_WAIT_TIME - chargeTimer.getTime()) / SPIKE_WAIT_TIME,
					20 * (SPIKE_WAIT_TIME - chargeTimer.getTime()) / SPIKE_WAIT_TIME,
					SDL_MapRGB( target->format, 255, 0, 0));
	}
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
