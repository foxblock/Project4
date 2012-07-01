#include "SpawnNormal.h"

#include "Events.h"
#include "gameDefines.h"
#include "StateLevel.h"
#include "UtilityFunctions.h"

// Unit classes
#include "UnitPlayer.h"
#include "UnitSpike.h"
#include "UnitLaser.h"
#include "UnitBomb.h"

// Actual value: 200
#define SPAWN_PLAYER_SAFE_RADIUS_SQR 40000.0f

#define UNIT_TYPE_COUNT 2

#define SPAWN_CORNER_WIDTH 200.0f
#define SPAWN_CORNER_HEIGHT 100.0f
#define SPAWN_CENTER_RADIUS 100.0f
#define SPAWN_SIDE_WIDTH 100.0f
#define SPAWN_SIDE_HEIGHT 50.0f
#define SPAWN_SIDE_OFFSET_H 30.0f
#define SPAWN_SIDE_OFFSET_V 15.0f

#define SPAWN_TIME_START 1000
#define SPAWN_MAX_START 10

SpawnNormal::SpawnNormal( StateLevel *newParent ) : SpawnBase( newParent )
{
	corner[0].pos = Vector2d<float>( SPAWN_CORNER_WIDTH / 2, SPAWN_CORNER_HEIGHT / 2 );
	corner[1].pos = Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_CORNER_WIDTH / 2, SPAWN_CORNER_HEIGHT / 2 );
	corner[2].pos = Vector2d<float>( SPAWN_CORNER_WIDTH / 2, APP_SCREEN_HEIGHT - SPAWN_CORNER_HEIGHT / 2 );
	corner[3].pos = Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_CORNER_WIDTH / 2, APP_SCREEN_HEIGHT - SPAWN_CORNER_HEIGHT / 2 );
	for ( int I = 0; I < ARRAY_SIZE( corner ); ++I )
		corner[I].size = Vector2d<float>( SPAWN_CORNER_WIDTH, SPAWN_CORNER_HEIGHT );
	side[0].pos = Vector2d<float>( APP_SCREEN_WIDTH / 2.0f, SPAWN_SIDE_OFFSET_V + SPAWN_SIDE_HEIGHT / 2.0f );
	side[1].pos = Vector2d<float>( APP_SCREEN_WIDTH / 2.0f, APP_SCREEN_HEIGHT - SPAWN_SIDE_OFFSET_V - SPAWN_SIDE_HEIGHT / 2.0f );
	side[2].pos = Vector2d<float>( SPAWN_SIDE_OFFSET_H + SPAWN_SIDE_HEIGHT / 2.0f, APP_SCREEN_HEIGHT / 2.0f );
	side[3].pos = Vector2d<float>( APP_SCREEN_WIDTH - SPAWN_SIDE_OFFSET_H - SPAWN_SIDE_HEIGHT / 2.0f, APP_SCREEN_HEIGHT / 2.0f );
	side[0].size = Vector2d<float>( SPAWN_SIDE_WIDTH, SPAWN_SIDE_HEIGHT );
	side[1].size = side[0].size;
	side[2].size = Vector2d<float>( SPAWN_SIDE_HEIGHT, SPAWN_SIDE_WIDTH );
	side[3].size = side[2].size;
	center.pos = Vector2d<float>( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 );
	center.radius = SPAWN_CENTER_RADIUS;
	timers.push_back( &spawnTimer );
}

SpawnNormal::~SpawnNormal()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int SpawnNormal::update( Uint32 delta )
{
	SpawnBase::update( delta );

	if ( !spawnTimer.isStopped() ||
			parent->countUnits() >= SPAWN_MAX_START )
		return 0;

	UnitBase *newUnit = NULL;

	if ( parent->player->shape.checkCollision( &center ) )
	{
		newUnit = new UnitSpike( parent );
	}
	for ( int I = 0; I < ARRAY_SIZE( corner ); ++I )
	{
		if ( parent->player->shape.checkCollision( &corner[I] ) )
		{
			newUnit = new UnitLaser( parent );
			break;
		}
	}
	for ( int I = 0; I < ARRAY_SIZE( side ); ++I )
	{
		if ( parent->player->shape.checkCollision( &side[I] ) )
		{
			newUnit = new UnitBomb( parent );
			break;
		}
	}

	if ( !newUnit )
		return 0;

	parent->addUnit( newUnit );

	*newUnit->x = rand() % APP_SCREEN_WIDTH;
	float temp = 0;
	do
	{
		*newUnit->y = rand() % APP_SCREEN_HEIGHT;
		temp = Utility::sqr( *newUnit->x - *parent->player->x ) +
				Utility::sqr( *newUnit->y - * parent->player->y );
	}
	while ( temp < SPAWN_PLAYER_SAFE_RADIUS_SQR );

	spawnTimer.start( SPAWN_TIME_START );
	EventUnitSpawn *event = new EventUnitSpawn( newUnit );
	parent->addEvent( event );

	return 0;
}

void SpawnNormal::render( SDL_Surface *target )
{
	for ( int I = 0; I < ARRAY_SIZE( corner ); ++I )
		corner[I].render( target, spGetRGB( 228, 0, 228 ) );
	for ( int I = 0; I < ARRAY_SIZE( side ); ++I )
		side[I].render( target, spGetRGB( 228, 0, 228 ) );
	center.render( target, spGetRGB( 228, 0, 228 ) );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
