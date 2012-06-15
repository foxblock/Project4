#include "StateCollision.h"

#include "gameDefines.h"
#include "UnitSpike.h"
#include "UnitLaser.h"
#include "ProjectileLaser.h"

#define PLAYER_VELOCITY 0.1f

StateCollision::StateCollision()
{
	unit1 = new UnitSpike( NULL );
	*unit1->x = APP_SCREEN_WIDTH / 2;
	*unit1->y = APP_SCREEN_HEIGHT / 4;
	unit2 = new UnitLaser( NULL );
	*unit2->x = APP_SCREEN_WIDTH / 4;
	*unit2->y = APP_SCREEN_HEIGHT /2;
	unit3 = new ProjectileLaser( NULL, -1 );
	*unit3->x = APP_SCREEN_WIDTH;
	*unit3->y = APP_SCREEN_HEIGHT / 2;
	unit4 = new ProjectileLaser( NULL, -1 );
	*unit4->x = 0;
	*unit4->y = APP_SCREEN_HEIGHT / 2;
	unit4->shape.pos.x = APP_SCREEN_WIDTH;
	player = unit1;

	font = spFontLoad( "fonts/lato.ttf", 12 );
	if ( font )
		spFontAddRange( font, ' ', '~', SDL_MapRGB( spGetWindowSurface()->format, 255, 0, 0 ) );
}

StateCollision::~StateCollision()
{
	delete unit1;
	delete unit2;
	delete unit3;
}


///--- PUBLIC ------------------------------------------------------------------

int StateCollision::update( Uint32 delta )
{
	delta = std::min((int)delta, MAX_DELTA);
	debugString.clear();

	handleInput( delta );

	if ( spGetInput()->button[SP_BUTTON_START] )
		return 1; // switch to level state

	unit1->update( delta );
	unit2->update( delta );
	unit3->update( delta );
	unit4->update( delta );

	if ( player != unit1 && player->checkCollision( unit1 ) )
		debugString += "Spike\n";
	if ( player != unit2 && player->checkCollision( unit2 ) )
		debugString += "Laser\n";
	if ( player != unit3 && player->checkCollision( unit3 ) )
		debugString += "Projectile left\n";
	if ( player != unit4 && player->checkCollision( unit4 ) )
		debugString += "Projectile right\n";

	return 0;
}

void StateCollision::render( SDL_Surface *target )
{
	unit1->render( target );
	unit2->render( target );
	unit3->render( target );
	unit4->render( target );

	spFontDraw( 10, 10, -1, debugString.c_str(), font );
}

void StateCollision::handleInput( Uint32 delta )
{
	if ( spGetInput()->button[SP_BUTTON_A] != 0 )
	{
		if ( player == unit1 )
			player = unit2;
		else if ( player == unit2 )
			player = unit3;
		else if ( player == unit3 )
			player = unit4;
		else if ( player == unit4 )
			player = unit1;
		spGetInput()->button[SP_BUTTON_A] = 0;
	}

	if ( spGetInput()->axis[0] < 0 )
	{
		player->vel.x = -PLAYER_VELOCITY * delta;
	}
	else if ( spGetInput()->axis[0] > 0 )
	{
		player->vel.x = PLAYER_VELOCITY * delta;
	}
	else
	{
		player->vel.x = 0;
	}
	if ( spGetInput()->axis[1] < 0 )
	{
		player->vel.y = PLAYER_VELOCITY * delta;
	}
	else if ( spGetInput()->axis[1] > 0 )
	{
		player->vel.y = -PLAYER_VELOCITY * delta;
	}
	else
	{
		player->vel.y = 0;
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

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
