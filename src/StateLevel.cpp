#include "StateLevel.h"

#include <cmath>

#include "gameDefines.h"
#include "UtilityFunctions.h"

#include "UnitSpike.h"
#include "UnitLaser.h"

#define UNIT_TYPE_COUNT 2
#define PLAYER_SAFE_RADIUS 200

StateLevel::StateLevel() : StateBase()
{
	player = new PLAYER_CLASS( this );
	*( player->x ) = 300;
	*( player->y ) = 300;
	player->invincible = true;

	debugText = spFontLoad( "fonts/lato.ttf", 12 );
	spFontAddRange( debugText, ' ', '~', SDL_MapRGB( spGetWindowSurface()->format, 255, 0, 0 ) );
//
//	spawnTimer.start();
//	spawnTimer.pause();
//
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
#ifdef _DEBUG
	debugString = Utility::numToStr( spGetFPS() ) + " fps\n";
#endif

	if (player)
		handleInput();

	if (player)
		player->update( delta );

	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
	{
		if (player)
			( *I )->ai( player );
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
			delete *I;
			units.erase( I );
		}
		else
			++I;
	}

	units.insert(units.end(),queue.begin(), queue.end());
	queue.clear();

	if ( spawnTimer.getStatus() == -1 )
	{
		spawnTimer.start( 1000 );

		int type = rand() % UNIT_TYPE_COUNT;
		UnitBase *newUnit = NULL;

		switch ( type )
		{
		case 0:
			newUnit = new UnitSpike( this );
			break;
		case 1:
			newUnit = new UnitLaser( this );
			break;
		}
		units.push_back( newUnit );

		*units.back()->x = rand() % APP_SCREEN_WIDTH;
		float temp = 0;
		do
		{
			*units.back()->y = rand() % APP_SCREEN_HEIGHT;
			if (player)
				temp = sqrt( pow( *units.back()->x - *player->x, 2 ) + pow( *units.back()->y - * player->y, 2 ) );
		}
		while ( temp < PLAYER_SAFE_RADIUS );
	}

#ifdef _DEBUG
	debugString += Utility::numToStr( units.size() ) + " units\n";
#endif

	if ( player && player->toBeRemoved )
		return 1;

	return 0;
}

void StateLevel::render( SDL_Surface *target )
{
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		( *I )->render( target );

	if (player)
		player->render( target );

#ifdef _DEBUG
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
	{
		if ( ( *I )->debugString[0] != 0 )
			debugString += ( *I )->debugString + "\n";
	}
	spFontDraw( 5, 5, -1, debugString.c_str(), debugText );
#endif
}

void StateLevel::addUnit(UnitBase *newUnit)
{
	queue.push_back(newUnit);
}

///--- PROTECTED ---------------------------------------------------------------

void StateLevel::handleInput()
{
	if ( spGetInput()->axis[0] != 0 )
	{
		*player->x += spGetInput()->axis[0];
	}
	if ( spGetInput()->axis[1] != 0 )
	{
		*player->y -= spGetInput()->axis[1];
	}

	if ( *player->x < 0 )
		*player->x = 0;
	else if ( *player->x > APP_SCREEN_WIDTH )
		*player->x = APP_SCREEN_WIDTH;
	if ( *player->y < 0 )
		*player->y = 0;
	else if ( *player->y > APP_SCREEN_HEIGHT )
		*player->y = APP_SCREEN_HEIGHT;
}

///--- PRIVATE -----------------------------------------------------------------
