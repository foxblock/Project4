#include "UnitPlayer.h"

#include "gameDefines.h"
#include "UtilityFunctions.h"

// Pixels per millisecond
#define PLAYER_MAX_VELOCITY 0.5f
#define PLAYER_ACCEL 0.01f

UnitPlayer::UnitPlayer( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	idle = spNewSprite();
	image = spLoadSurface( "images/units/coolface.png" );
	spNewSubSpriteWithTiling( idle, image, 0, 0, 32, 32, 1000 );
	activeSprite = idle;
	shape.radius = 16;
	x = &( shape.pos.x );
	y = &( shape.pos.y );
	maxVel = PLAYER_MAX_VELOCITY;
}

UnitPlayer::~UnitPlayer()
{
	spDeleteSprite( idle );
	SDL_FreeSurface( image );
}


///--- PUBLIC ------------------------------------------------------------------

int UnitPlayer::update( Uint32 delta )
{
	Vector2d<float> dir(spGetInput()->axis[0],-spGetInput()->axis[1]);
	accel = dir.unit() * PLAYER_ACCEL;

	if ( *x < 0 )
		*x = 0;
	else if ( *x > APP_SCREEN_WIDTH )
		*x = APP_SCREEN_WIDTH;
	if ( *y < 0 )
		*y = 0;
	else if ( *y > APP_SCREEN_HEIGHT )
		*y = APP_SCREEN_HEIGHT;

	UnitBase::update( delta );

	return 0;
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
