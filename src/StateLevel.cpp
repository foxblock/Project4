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
	player = new UnitSpike( this );
	player->x = 300;
	player->y = 300;

	debugText = spFontLoad( "fonts/lato.ttf", 24 );
	spFontAddRange( debugText, ' ', '~', SDL_MapRGB( spGetWindowSurface()->format, 255, 0, 0 ) );
}

StateLevel::~StateLevel()
{
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		delete *I;
	units.clear();
	delete player;
}


///--- PUBLIC ------------------------------------------------------------------

int StateLevel::update( Uint32 delta )
{
#ifdef _DEBUG
	debugString = Utility::numToStr(spGetFPS()) + " fps | ";
#endif

	handleInput();

	player->update( delta );

	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); )
	{
		if ( ( *I )->toBeRemoved )
		{
			units.erase( I );
		}
		else
		{
			( *I )->ai( player );
			( *I )->update( delta );
			for ( std::vector<UnitBase *>::iterator K = units.begin(); K != units.end(); ++K )
			{
				if ( *I == *K )
					continue;

				if ( checkForCollision( *I, *K ) )
				{
					( *I )->toBeRemoved = ( *K )->deadlyOnTouch && !( *I )->invincible;
					( *K )->toBeRemoved = ( *I )->deadlyOnTouch && !( *K )->invincible;
				}
				if ( ( *I )->toBeRemoved )
					break;
			}
			if ( checkForCollision( *I, player ) )
			{
				( *I )->toBeRemoved = player->deadlyOnTouch && !( *I )->invincible;
				player->toBeRemoved = ( *I )->deadlyOnTouch && !player->invincible;
			}

			++I;
		}
	}

	if (spawnTimer.getStatus() == -1)
	{
		spawnTimer.start( 1000 );

		int type = rand() % UNIT_TYPE_COUNT;
		UnitBase* newUnit = NULL;
		switch (type)
		{
		case 0:
			newUnit = new UnitSpike( this );
			break;
		case 1:
			newUnit = new UnitLaser( this );
			break;
		}
		units.push_back( newUnit );

		units.back()->x = rand() % APP_SCREEN_WIDTH;
		float temp = 0;
		do
		{
			units.back()->y = rand() % APP_SCREEN_HEIGHT;
			temp = sqrt( pow(units.back()->x - player->x,2) + pow(units.back()->y - player->y,2) );
		} while ( temp < PLAYER_SAFE_RADIUS );
	}

#ifdef _DEBUG
	debugString += Utility::numToStr(units.size()) + " units | ";
#endif

	if ( player->toBeRemoved )
		return 1;

	return 0;
}

void StateLevel::render( SDL_Surface *target )
{
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		( *I )->render( target );

	player->render( target );

	#ifdef _DEBUG
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
	{
		if ((*I)->debugString[0] != 0)
			debugString += (*I)->debugString + " | ";
	}
	spFontDraw(5,5,-1,debugString.c_str(),debugText);
	#endif
}

///--- PROTECTED ---------------------------------------------------------------

void StateLevel::handleInput()
{
	if ( spGetInput()->axis[0] != 0 )
	{
		player->x += spGetInput()->axis[0];
	}
	if ( spGetInput()->axis[1] != 0 )
	{
		player->y -= spGetInput()->axis[1];
	}

	if (player->x < 0)
		player->x = 0;
	else if (player->x > APP_SCREEN_WIDTH)
		player->x = APP_SCREEN_WIDTH;
	if (player->y < 0)
		player->y = 0;
	else if (player->y > APP_SCREEN_HEIGHT)
		player->y = APP_SCREEN_HEIGHT;
}

bool StateLevel::checkForCollision( UnitBase *unitA, UnitBase *unitB )
{
	if ( ( ( unitA->x - unitA->getWidth() / 2.0f < unitB->x + unitB->getWidth() / 2.0f ) &&
			( unitB->x - unitB->getWidth() / 2.0f < unitA->x + unitA->getWidth() / 2.0f ) ) &&
			( ( unitA->y - unitA->getHeight() / 2.0f < unitB->y + unitB->getHeight() / 2.0f ) &&
			  ( unitB->y - unitB->getHeight() / 2.0f < unitA->y + unitA->getHeight() / 2.0f ) ) )
		return true;
	return false;
}

///--- PRIVATE -----------------------------------------------------------------
