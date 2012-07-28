#include "UnitPlayer.h"

#include "gameDefines.h"
#include "UtilityFunctions.h"

// Pixels per millisecond
#define PLAYER_MAX_VELOCITY 0.5f
#define PLAYER_ACCEL 0.01f
#define PLAYER_RADIUS 16

SDL_Surface* UnitPlayer::idle = NULL;

UnitPlayer::UnitPlayer( StateLevel *newParent ) : UnitBase( newParent, &shape )
{
	if ( !idle )
		generateIdleImage();
	activeSprite = idle;
	shape.radius = 16;
	x = &( shape.pos.x );
	y = &( shape.pos.y );
	maxVel = PLAYER_MAX_VELOCITY;
	maxAccel = PLAYER_ACCEL;
	type = utPlayer;
}

UnitPlayer::~UnitPlayer()
{
	//
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

void UnitPlayer::generateIdleImage()
{
	idle = spCreateSurface( PLAYER_RADIUS * 2, PLAYER_RADIUS * 2 );
	SDL_FillRect( idle, NULL, SP_ALPHA_COLOR );
	spSelectRenderTarget( idle );
	spEllipse( PLAYER_RADIUS, PLAYER_RADIUS, -1, PLAYER_RADIUS, PLAYER_RADIUS, spGetFastRGB( 255, 255, 255 ) );
	spSelectRenderTarget( spGetWindowSurface() );
}
