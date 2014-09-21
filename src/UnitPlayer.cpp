#include "UnitPlayer.h"

#include "gameDefines.h"
#include "UtilityFunctions.h"

#include "sparrowPrimitives.h"

// Pixels per millisecond
#define PLAYER_MAX_VELOCITY 0.5f
#define PLAYER_ACCEL 0.01f
#define PLAYER_DRAW_RADIUS 10
#define PLAYER_COLLISION_RADIUS 4

UnitPlayer::UnitPlayer( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	activeSprite = NULL;
	shape.radius = PLAYER_COLLISION_RADIUS;
	maxVel = PLAYER_MAX_VELOCITY;
	maxAccel = PLAYER_ACCEL;
	type = utPlayer;
	flags.add( ufIsPlayer );
	lastVel = Vector2d<float>(0,-1);
}

UnitPlayer::~UnitPlayer()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int UnitPlayer::update( const Uint32 &delta )
{
	if ( spGetInput()->axis[0] == 0 && spGetInput()->axis[1] == 0 )
	{
		Vector2d<float> dir(spGetInput()->analog_axis[0],spGetInput()->analog_axis[1]);
		dir /= (float)SP_ANALOG_AXIS_MAX;
		vel = dir * maxVel;
		accel.x = accel.y = 0;
	}
	else
	{
		Vector2d<float> dir(spGetInput()->axis[0],spGetInput()->axis[1]);
		accel = dir.unit() * maxAccel;
	}

	if ( *x < 0 )
		*x = 0;
	else if ( *x > APP_SCREEN_WIDTH )
		*x = APP_SCREEN_WIDTH;
	if ( *y < 0 )
		*y = 0;
	else if ( *y > APP_SCREEN_HEIGHT )
		*y = APP_SCREEN_HEIGHT;

	UnitBase::update( delta );

	if ( vel.x != 0 || vel.y != 0 )
		lastVel = vel;

	return 0;
}

void UnitPlayer::render( SDL_Surface *const target )
{
	float angle = lastVel.angle();
	spTriangle( *x + PLAYER_DRAW_RADIUS * cos( angle ),
				*y + PLAYER_DRAW_RADIUS * sin( angle ),
				-1,
				*x + PLAYER_DRAW_RADIUS * cos( angle + M_PI * 1.2 ),
				*y + PLAYER_DRAW_RADIUS * sin( angle + M_PI * 1.2 ),
				-1,
				*x + PLAYER_DRAW_RADIUS * cos( angle + M_PI * 0.8 ),
				*y + PLAYER_DRAW_RADIUS * sin( angle + M_PI * 0.8 ),
				-1,
				-1 );

	UnitBase::render( target );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
