#include "StateLevel.h"

#include "UnitSpike.h"

StateLevel::StateLevel() : StateBase()
{
	units.push_back( new UnitSpike( this ) );
	units.push_back( new UnitSpike( this ) );
	units.push_back( new UnitSpike( this ) );
	units.push_back( new UnitSpike( this ) );
	units.push_back( new UnitSpike( this ) );

	player = new UnitSpike( this );
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

	if ( player->toBeRemoved )
		return 1;

	return 0;
}

void StateLevel::render( SDL_Surface *target )
{
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		( *I )->render( target );

	player->render( target );
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
