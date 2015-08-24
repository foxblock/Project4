#include "StateCollision.h"

#include "gameDefines.h"
#include "UnitSpike.h"
#include "UnitLaser.h"
#include "ProjectileLaser.h"
#include "UtilityFunctions.h"

#include "sparrowPrimitives.h"

#define TEST_PLAYER_VELOCITY 0.2f

StateCollision::StateCollision()
{
	unit1 = new UnitSpike( NULL );
	*unit1->x = APP_SCREEN_WIDTH / 2;
	*unit1->y = APP_SCREEN_HEIGHT / 4;
	unit2 = new UnitLaser( NULL );
	*unit2->x = APP_SCREEN_WIDTH / 4;
	*unit2->y = APP_SCREEN_HEIGHT /2;
	unit22 = new UnitLaser( NULL );
	*unit22->x = APP_SCREEN_WIDTH / 4 + unit2->shape.radius * 2;
	*unit22->y = APP_SCREEN_HEIGHT /2;
	unit3 = new ProjectileLaser( NULL, -1 );
	*unit3->x = APP_SCREEN_WIDTH;
	*unit3->y = APP_SCREEN_HEIGHT / 2;
	unit4 = new ProjectileLaser( NULL, -1 );
	*unit4->x = 0;
	*unit4->y = APP_SCREEN_HEIGHT / 2;
	unit4->shape.pos.x = APP_SCREEN_WIDTH;
	player = unit1;

	font = spFontLoad( FONT_GENERAL, 12 );
	if ( font )
		spFontAdd( font, SP_FONT_GROUP_ASCII, spGetFastRGB( 255, 0, 0 ) );
	type = stCollision;
}

StateCollision::~StateCollision()
{
	delete unit1;
	delete unit2;
	delete unit22;
	delete unit3;
	delete unit4;
	spFontDelete( font );
	spResetButtonsState();
	spResetAxisState();
}

///--- PUBLIC ------------------------------------------------------------------

int StateCollision::update( Uint32 delta )
{
	StateBase::update( delta );

	delta = std::min((int)delta, MAX_DELTA);
	debugString.clear();

	handleInput( delta );

	if ( spGetInput()->button[SP_BUTTON_START] )
	{
		spResetButtonsState();
		return stMenu; // switch to menu state
	}

	unit1->update( delta );
	//unit2->update( delta );
	//unit22->update( delta );
	unit3->update( delta );
	unit4->update( delta );

	if ( player != unit1 && player->checkCollision( unit1 ) )
		debugString += "Spike\n";
	if ( player != unit2 && player->checkCollision( unit2 ) )
		debugString += "Laser\n";
	if ( player != unit22 && player->checkCollision( unit22 ) )
		debugString += "Laser2\n";
	unit2->checkCollision( player );
	unit22->checkCollision( player );
	if ( player != unit3 && player->checkCollision( unit3 ) )
		debugString += "Projectile left\n";
	if ( player != unit4 && player->checkCollision( unit4 ) )
		debugString += "Projectile right\n";

	float angle = (player->shape->pos - Vector2d<float>( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 )).angle();
	debugString += "Angle: " + Utility::numToStr( angle * 180 / M_PI ) + "\n";

	return 0;
}

void StateCollision::render( SDL_Surface *target )
{
	spClearTarget( COLOUR_BACKGROUND );

	if (unit1 != player)
		unit1->render( target );
	if (unit2 != player)
		unit2->render( target );
	if (unit22 != player)
		unit22->render( target );
	if (unit3 != player)
		unit3->render( target );
	if (unit4 != player)
		unit4->render( target );
	player->render( target );

	spFontDraw( 10, 10, -1, (unsigned char*) debugString.c_str(), font );

	spLine( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 * 0.8f, -1,
			APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 * 1.2f, -1, -1 );
	spLine( APP_SCREEN_WIDTH / 2 * 0.8f, APP_SCREEN_HEIGHT / 2, -1,
			APP_SCREEN_WIDTH / 2 * 1.2f, APP_SCREEN_HEIGHT / 2, -1, -1 );
	spLine( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2, -1,
			*player->x, *player->y, -1, spGetRGB( 228, 228, 288 ) );
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
		player->vel.x = -TEST_PLAYER_VELOCITY;
	}
	else if ( spGetInput()->axis[0] > 0 )
	{
		player->vel.x = TEST_PLAYER_VELOCITY;
	}
	else
	{
		player->vel.x = 0;
	}
	if ( spGetInput()->axis[1] > 0 )
	{
		player->vel.y = TEST_PLAYER_VELOCITY;
	}
	else if ( spGetInput()->axis[1] < 0 )
	{
		player->vel.y = -TEST_PLAYER_VELOCITY;
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
