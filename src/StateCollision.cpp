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
	UnitBase *temp = new UnitSpike(NULL);
	*temp->x = APP_SCREEN_WIDTH / 2;
	*temp->y = APP_SCREEN_HEIGHT / 4;
	temp->flags.add(UnitBase::ufInvincible);
	player = temp;
	addUnit(temp);
	temp = new UnitLaser(NULL);
	*temp->x = APP_SCREEN_WIDTH / 4;
	*temp->y = APP_SCREEN_HEIGHT / 2;
	temp->flags.add(UnitBase::ufInvincible);
	((UnitLaser*)temp)->stationary = true;
	addUnit(temp);
	float tempPos = *temp->x + ((UnitLaser*)temp)->shape.radius * 2;
	temp = new UnitLaser(NULL);
	*temp->x = tempPos;
	*temp->y = APP_SCREEN_HEIGHT / 2;
	temp->flags.add(UnitBase::ufInvincible);
	((UnitLaser*)temp)->stationary = true;
	addUnit(temp);
	temp = new ProjectileLaser(NULL, -1);
	*temp->x = APP_SCREEN_WIDTH;
	*temp->y = APP_SCREEN_HEIGHT / 2;
	temp->flags.add(UnitBase::ufInvincible);
	addUnit(temp);
	temp = new ProjectileLaser(NULL, -1);
	*temp->x = 0;
	*temp->y = APP_SCREEN_HEIGHT / 2;
	temp->shape->pos.x = APP_SCREEN_WIDTH;
	temp->flags.add(UnitBase::ufInvincible);
	addUnit(temp);

	font = spFontLoad(FONT_GENERAL, 12);
	if (font)
		spFontAdd(font, SP_FONT_GROUP_ASCII, spGetFastRGB(255, 0, 0));
	type = stCollision;
}

StateCollision::~StateCollision()
{
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		delete *I;
	units.clear();
	for ( std::vector<UnitBase *>::iterator I = unitQueue.begin(); I != unitQueue.end(); ++I )
		delete *I;
	unitQueue.clear();
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

	// Unit update, collision checking (creates events)
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
	{
		if ( (*I)->flags.has( UnitBase::ufFrozen ) )
			continue;

//		if ( player )
//			( *I )->ai( delta, player );
		( *I )->update( delta );
		for ( std::vector<UnitBase *>::iterator K = units.begin(); K != units.end(); ++K )
		{
			if ( *I == *K )
				continue;
			if ( ( *I )->checkCollision( *K ) )
			{
				debugString += Utility::numToStr((*I)->type) + "->" + Utility::numToStr((*K)->type) + "\n";
				( *I )->collisionResponse( *K );
				( *K )->collisionResponse( *I );
			}
		}
	}

	units.insert( units.end(), unitQueue.begin(), unitQueue.end() );
	unitQueue.clear();

	// Unit handling (adding, removing)
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); )
	{
		if ( ( *I )->toBeRemoved )
		{
			delete *I;
			I = units.erase( I );
		}
		else
			++I;
	}

	float angle = (player->shape->pos - Vector2d<float>( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 )).angle();
	debugString += "Angle: " + Utility::numToStr( angle * 180 / M_PI ) + "\n";

	return 0;
}

void StateCollision::addUnit(UnitBase *newUnit)
{
#ifdef _DEBUG
	printf("%s:%i\t\t%s, type: %i\n",__FILE__,__LINE__,"New unit is being added",newUnit->type);
#endif
	unitQueue.push_back( newUnit );
}

void StateCollision::render(SDL_Surface *target)
{
	spClearTarget(COLOUR_BACKGROUND);

	for (std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I)
		(*I)->render(target);

	if (player)
		player->render(target);

	spFontDraw(10, 10, -1, (unsigned char*) debugString.c_str(), font);

	spLine(APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 * 0.8f, -1,
			APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 * 1.2f, -1, -1);
	spLine(APP_SCREEN_WIDTH / 2 * 0.8f, APP_SCREEN_HEIGHT / 2, -1,
			APP_SCREEN_WIDTH / 2 * 1.2f, APP_SCREEN_HEIGHT / 2, -1, -1);
	spLine(APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2, -1,
			*player->x, *player->y, -1, spGetRGB(228, 228, 288));
}

void StateCollision::handleInput( Uint32 delta )
{
	if ( spGetInput()->button[SP_BUTTON_A] != 0 )
	{
		for (int I = 0; I < units.size(); ++I)
		{
			if (units[I] == player)
			{
				player = (I == units.size() - 1) ? units[0] : units[I+1];
				break;
			}
		}
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
