#include "StateLevel.h"

#include <cmath>

#include "gameDefines.h"
#include "UtilityFunctions.h"

#include "UnitPlayer.h"
#include "UnitSpike.h"
#include "UnitLaser.h"

// Actual value: 200
#define PLAYER_SAFE_RADIUS_SQR 40000.0f

#define UNIT_TYPE_COUNT 2

#define LEVEL_CORNER_WIDTH 200.0f
#define LEVEL_CORNER_HEIGHT 100.0f
#define LEVEL_CENTER_RADIUS 100.0f

#define LEVEL_SPAWN_TIME 1000
#define LEVEL_SPAWN_MAX 10

StateLevel::StateLevel() : StateBase()
{
	player = new PLAYER_CLASS( this );
	*( player->x ) = APP_SCREEN_WIDTH / 2;
	*( player->y ) = APP_SCREEN_HEIGHT / 2;
//	player->props.addFlag( UnitBase::ufInvincible );

#ifdef _DEBUG
	debugText = spFontLoad( GAME_FONT, 12 );
	if ( debugText )
		spFontAddRange( debugText, ' ', '~', spGetRGB( 255, 0, 0 ) );
#endif
	killText = spFontLoad( GAME_FONT, 32 );
	if ( killText )
		spFontAddRange( killText, '0', '9', spGetRGB( 255, 255, 255 ) );
	kills = 0;

	corner[0].pos = Vector2d<float>(LEVEL_CORNER_WIDTH / 2,LEVEL_CORNER_HEIGHT / 2);
	corner[1].pos = Vector2d<float>(APP_SCREEN_WIDTH - LEVEL_CORNER_WIDTH / 2,LEVEL_CORNER_HEIGHT / 2);
	corner[2].pos = Vector2d<float>(LEVEL_CORNER_WIDTH / 2,APP_SCREEN_HEIGHT -LEVEL_CORNER_HEIGHT / 2);
	corner[3].pos = Vector2d<float>(APP_SCREEN_WIDTH - LEVEL_CORNER_WIDTH / 2,APP_SCREEN_HEIGHT - LEVEL_CORNER_HEIGHT / 2);
	for ( int I = 0; I < ARRAY_SIZE(corner); ++I)
		corner[I].size = Vector2d<float>(LEVEL_CORNER_WIDTH, LEVEL_CORNER_HEIGHT);
	center.pos = Vector2d<float>(APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2);
	center.radius = LEVEL_CENTER_RADIUS;

//	spawnTimer.start();
//	spawnTimer.pause();

//	ProjectileLaser* temp = new ProjectileLaser(this,1000);
//	units.push_back(temp);
//	temp->shape.pos = Vector2d<float>(0,0);
//	temp->shape.target = Vector2d<float>(600,600);
//	temp = new ProjectileLaser(this,1000);
//	units.push_back(temp);
//	temp->shape.pos = Vector2d<float>(0,600);
//	temp->shape.target = Vector2d<float>(600,0);
}

StateLevel::~StateLevel()
{
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		delete *I;
	units.clear();
	for ( std::vector<UnitBase *>::iterator I = queue.begin(); I != queue.end(); ++I )
		delete *I;
	queue.clear();
	delete player;
}


///--- PUBLIC ------------------------------------------------------------------

int StateLevel::update( Uint32 delta )
{
	delta = std::min( ( int )delta, MAX_DELTA );

#ifdef _DEBUG
	debugString = Utility::numToStr( spGetFPS() ) + " fps (" + Utility::numToStr(delta) + ")\n";
#endif

	if ( player )
		player->update( delta );

	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
	{
		if ( player )
			( *I )->ai( delta, player );
		( *I )->update( delta );
		for ( std::vector<UnitBase *>::iterator K = I + 1; K != units.end(); ++K )
		{
			if ( ( *I )->checkCollision( *K ) )
			{
				( *I )->collisionResponse( *K );
				( *K )->collisionResponse( *I );
			}
		}
		if ( player && ( *I )->checkCollision( player ) )
		{
			( *I )->collisionResponse( player );
			player->collisionResponse( *I );
		}
	}

	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); )
	{
		if ( ( *I )->toBeRemoved )
		{
			++kills;
			delete *I;
			units.erase( I );
		}
		else
			++I;
	}

	units.insert( units.end(), queue.begin(), queue.end() );
	queue.clear();

	spawnUnits( delta );

#ifdef _DEBUG
	debugString += Utility::numToStr( units.size() ) + " units\n";
#endif

	if ( player && player->toBeRemoved )
	{
		printf("Kill streak: %i\n",kills);
		return 1;
	}

	return 0;
}

void StateLevel::render( SDL_Surface *target )
{
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		( *I )->render( target );

	if ( player )
		player->render( target );

	if ( killText )
		spFontDrawRight( APP_SCREEN_WIDTH - 5, APP_SCREEN_HEIGHT - 40, -1, Utility::numToStr(kills).c_str(), killText );

#ifdef _DEBUG
	if ( player && player->debugString[0] != 0)
		debugString += player->debugString + "\n";
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
	{
		if ( ( *I )->debugString[0] != 0 )
			debugString += ( *I )->debugString + "\n";
	}
	if ( debugText )
		spFontDraw( 5, 5, -1, debugString.c_str(), debugText );
	for ( int I = 0; I < ARRAY_SIZE(corner); ++I )
		corner[I].render( target, spGetRGB( 228, 0, 228 ) );
	center.render( target, spGetRGB( 228, 0, 228 ) );
#endif
}

void StateLevel::addUnit( UnitBase *newUnit )
{
	queue.push_back( newUnit );
}

///--- PROTECTED ---------------------------------------------------------------

void StateLevel::spawnUnits( Uint32 delta )
{
	if ( spawnTimer.getStatus() != -1 ||
		units.size() >= LEVEL_SPAWN_MAX )
		return;

	UnitBase *newUnit = NULL;

	if ( player->shape.checkCollision( &center ) )
	{
		newUnit = new UnitSpike( this );
	}
	for ( int I = 0; I < ARRAY_SIZE( corner ); ++I)
	{
		if ( player->shape.checkCollision( &corner[I] ) )
		{
			newUnit = new UnitLaser( this );
			break;
		}
	}

	if ( !newUnit )
		return;

	units.push_back( newUnit );

	*newUnit->x = rand() % APP_SCREEN_WIDTH;
	float temp = 0;
	do
	{
		*newUnit->y = rand() % APP_SCREEN_HEIGHT;
		if ( player )
			temp = Utility::sqr( *newUnit->x - *player->x ) + Utility::sqr( *newUnit->y - * player->y );
	}
	while ( temp < PLAYER_SAFE_RADIUS_SQR );

	spawnTimer.start(LEVEL_SPAWN_TIME);
}

///--- PRIVATE -----------------------------------------------------------------
