#include "UnitSpike.h"

#include <cmath>
#include "UtilityFunctions.h"
#include "gameDefines.h"

#include "sparrowPrimitives.h"

// Pixels per millisecond
#define SPIKE_ATTACK_ACCEL 0.008f
#define SPIKE_CHARGE_MAX_VEL 0.75f
#define SPIKE_MOVEMENT_MAX_VEL 0.5f
#define SPIKE_IDLE_MAX_VEL 0.1f
#define SPIKE_IDLE_FRICTION 0.001f
#define SPIKE_IDLE_MAX_ACCEL 0.005f
#define SPIKE_IDLE_ACCEL 0.00005f
// 200 pixels
#define SPIKE_ATTACK_RADIUS_SQR 40000.0f
// 75 pixels
#define SPIKE_CHARGE_RADIUS_SQR 5625.0f
#define SPIKE_WAIT_TIME 500
#define SPIKE_CHARGE_TIME 500
#define SPIKE_SPIKE_COUNT 16
#define SPIKE_IDLE_RADIUS 10
#define SPIKE_ATTACK_RADIUS 16

SDL_Surface* UnitSpike::idle = NULL;
SDL_Surface* UnitSpike::attack = NULL;

UnitSpike::UnitSpike( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	if ( !idle )
		generateIdleImage();
	if ( !attack )
		generateAttackImage();
	activeSprite = idle;
	shape.radius = SPIKE_IDLE_RADIUS;
	chargeState = 0;
	maxVel = SPIKE_IDLE_MAX_VEL;
	maxAccel = SPIKE_IDLE_MAX_ACCEL;
	friction = SPIKE_IDLE_FRICTION;
	type = utSpike;
	timers.push_back( &chargeTimer );
	target = Vector2d<float>(-1,-1);
}

UnitSpike::~UnitSpike()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

void UnitSpike::ai( const Uint32 &delta, UnitBase *const player )
{
	// move to specific target
	if ( !shape.pos.isInRect( Vector2d<int>(0,0), Vector2d<int>( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT ) ) &&
		target.x < 0 && target.y < 0 )
	{
		// spawn outside -> set target inside view
		target = Vector2d<float>( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 );
		float dist = std::sqrt( Utility::sqr( shape.pos.x - Utility::clamp( (int)shape.pos.x, 0, APP_SCREEN_WIDTH ) ) +
								Utility::sqr( shape.pos.y - Utility::clamp( (int)shape.pos.y, 0, APP_SCREEN_HEIGHT ) ) );
		target = shape.pos + (target - shape.pos).unit() * ( dist + shape.radius );
	}
	if ( target.x >= 0 && target.y >= 0 )
	{
		maxVel = SPIKE_IDLE_MAX_VEL;
		maxAccel = SPIKE_IDLE_MAX_ACCEL;
		friction = SPIKE_IDLE_FRICTION;
		accel = (target - shape.pos).unit() * SPIKE_IDLE_MAX_ACCEL * delta;
		if ( Utility::floatComp( target, shape.pos ) )
			target = Vector2d<float>(-1,-1);
		else
			return;
	}

	Vector2d<float> diff( *player->x - *x, *player->y - *y );
	float dist = diff.lengthSquared();
	// waiting for charge -> charging
	if ( chargeTimer.stopped() && chargeState == 1 )
	{
		maxVel = UNIT_DEFAULT_MAX_VEL;
		vel = diff.unit() * SPIKE_CHARGE_MAX_VEL;
		friction = 0;
		activeSprite = attack;
		flags.add( ufDeadlyOnTouch );
		chargeState = 2;
		chargeTimer.start( SPIKE_CHARGE_TIME );
	}
	// prevent charging unit from going off screen
	if ( !chargeTimer.stopped() && chargeState == 2 &&
		!shape.pos.isInRect(Vector2d<float>(0,0),Vector2d<float>(APP_SCREEN_WIDTH,APP_SCREEN_HEIGHT)) )
	{
		chargeTimer.stop();
		*x = Utility::clamp( *x, 0.0f, (float)APP_SCREEN_WIDTH );
		*y = Utility::clamp( *y, 0.0f, (float)APP_SCREEN_HEIGHT );
	}
	// charging -> idle movement
	if ( chargeTimer.stopped() && chargeState == 2 )
	{
		chargeState = 0;
		activeSprite = idle;
		flags.remove( ufDeadlyOnTouch );
		vel = Vector2d<float>( 0, 0 );
		accel = Vector2d<float>( 0, 0 );
	}
	// idle
	if ( chargeState == 0 )
	{
		// idle -> charging
		if ( dist < SPIKE_CHARGE_RADIUS_SQR )
		{
			chargeTimer.start( SPIKE_WAIT_TIME );
			chargeState = 1;
			vel = Vector2d<float>( 0, 0 );
			accel = Vector2d<float>( 0, 0 );
		}
		// idle -> following
		else if ( dist < SPIKE_ATTACK_RADIUS_SQR )
		{
			maxVel = SPIKE_MOVEMENT_MAX_VEL;
			friction = UNIT_DEFAULT_FRICTION;
			maxAccel = UNIT_DEFAULT_MAX_ACCEL;
			accel = diff.unit() * SPIKE_ATTACK_ACCEL;
		}
		// idle movement
		else
		{
			maxVel = SPIKE_IDLE_MAX_VEL;
			maxAccel = SPIKE_IDLE_MAX_ACCEL;
			friction = SPIKE_IDLE_FRICTION;
			Vector2d<float> temp( Utility::randomRange( -1, 1 ),  Utility::randomRange( -1, 1 ) );
			accel += temp.unit() * SPIKE_IDLE_ACCEL * delta;
			if ( (*x < shape.radius && accel.x < 0) ||
					(*x > APP_SCREEN_WIDTH - shape.radius && accel.x > 0 ) )
				accel.x *= -1;
			if ( ( *y < shape.radius && accel.y < 0 ) ||
					(*y > APP_SCREEN_HEIGHT - shape.radius && accel.y > 0 ) )
				accel.y *= -1;
		}
	}
}

void UnitSpike::render( SDL_Surface *const target )
{
	UnitBase::render( target );
	if ( chargeState == 1 )
	{
		spEllipse( *x, *y, -1, SPIKE_IDLE_RADIUS * ( SPIKE_WAIT_TIME - chargeTimer.getTime() ) / SPIKE_WAIT_TIME,
				   SPIKE_IDLE_RADIUS * ( SPIKE_WAIT_TIME - chargeTimer.getTime() ) / SPIKE_WAIT_TIME,
				   spGetFastRGB( 255, 0 , 0 ) );
	}
	#ifdef _DEBUG
	if ( this->target.x >= 0 && this->target.y >= 0 )
		spLine(*x,*y,-1,this->target.x,this->target.y,-1,spGetFastRGB( 255, 255, 0 ));
	#endif // _DEBUG
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------

void UnitSpike::generateIdleImage()
{
	idle = spCreateSurface( SPIKE_IDLE_RADIUS * 2, SPIKE_IDLE_RADIUS * 2 );
	SDL_FillRect( idle, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( idle );
	spEllipse( SPIKE_IDLE_RADIUS ,SPIKE_IDLE_RADIUS, -1, SPIKE_IDLE_RADIUS, SPIKE_IDLE_RADIUS, 0 );
	spSelectRenderTarget( spGetWindowSurface() );
}

void UnitSpike::generateAttackImage()
{
	attack = spCreateSurface( SPIKE_ATTACK_RADIUS * 2, SPIKE_ATTACK_RADIUS * 2 );
	SDL_FillRect( attack, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( attack );
	spEllipse( SPIKE_ATTACK_RADIUS, SPIKE_ATTACK_RADIUS, -1, SPIKE_IDLE_RADIUS, SPIKE_IDLE_RADIUS, spGetFastRGB(255,0,0) );
	const float spikeRadSize = 2.0f * M_PI / SPIKE_SPIKE_COUNT;
	for ( int I = 0; I < SPIKE_SPIKE_COUNT; ++I )
	{
		spTriangle( SPIKE_ATTACK_RADIUS + SPIKE_ATTACK_RADIUS * sin( spikeRadSize * I ),
					SPIKE_ATTACK_RADIUS - SPIKE_ATTACK_RADIUS * cos( spikeRadSize * I ), -1,
					SPIKE_ATTACK_RADIUS + (SPIKE_IDLE_RADIUS-1) * sin( spikeRadSize * (I - 0.5f) ),
					SPIKE_ATTACK_RADIUS - (SPIKE_IDLE_RADIUS-1) * cos( spikeRadSize * (I - 0.5f) ), -1,
					SPIKE_ATTACK_RADIUS + (SPIKE_IDLE_RADIUS-1) * sin( spikeRadSize * (I + 0.5f) ),
					SPIKE_ATTACK_RADIUS - (SPIKE_IDLE_RADIUS-1) * cos( spikeRadSize * (I + 0.5f) ), -1,
					spGetFastRGB(255,0,0) );
	}
	spSelectRenderTarget( spGetWindowSurface() );
}
