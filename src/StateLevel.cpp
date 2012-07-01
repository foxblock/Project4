#include "StateLevel.h"

#include <cmath>

#include "gameDefines.h"
#include "UtilityFunctions.h"

#include "UnitPlayer.h"

StateLevel::StateLevel() :
	StateBase(),
	scoreKeeper( this ),
	spawnHandler( this )
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

	spGetInput()->button[SP_BUTTON_START] = 0;

	type = stLevel;

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
	for ( std::vector<UnitBase *>::iterator I = unitQueue.begin(); I != unitQueue.end(); ++I )
		delete *I;
	unitQueue.clear();
	delete player;
}


///--- PUBLIC ------------------------------------------------------------------

int StateLevel::update( Uint32 delta )
{
	if ( spGetInput()->button[SP_BUTTON_START] )
		return -1;

	delta = std::min( ( int )delta, MAX_DELTA );

#ifdef _DEBUG
	debugString = Utility::numToStr( spGetFPS() ) + " fps (" + Utility::numToStr( delta ) + ")\n";
#endif

	// Unit update, collision checking (creates events)
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

	// Spawning (creates events)
	spawnHandler.update( delta );

	// Unit handling (adding, removing)
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

	units.insert( units.end(), unitQueue.begin(), unitQueue.end() );
	unitQueue.clear();

	// Score (reads events)
	scoreKeeper.update( delta );

	// Events (reads and removes events)
	handleEvents( delta );

#ifdef _DEBUG
	debugString += Utility::numToStr( units.size() ) + " units\n";
#endif

	if ( player && player->toBeRemoved )
	{
		printf( "Score: %i\n", scoreKeeper.getScore() );
		return stScore;
	}

	return 0;
}

void StateLevel::render( SDL_Surface *target )
{
	spClearTarget( spGetRGB( 128, 0, 0 ) );

	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		( *I )->render( target );

	if ( player )
		player->render( target );

	scoreKeeper.render( target );

#ifdef _DEBUG
	if ( debugText )
		spFontDraw( 5, 5, -1, debugString.c_str(), debugText );
	spawnHandler.render( target );
#endif
}

void StateLevel::addUnit( UnitBase *newUnit )
{
	unitQueue.push_back( newUnit );
}

void StateLevel::addEvent( EventBase *newEvent )
{
	eventQueue.push_back( newEvent );
}

///--- PROTECTED ---------------------------------------------------------------

void StateLevel::handleEvents( Uint32 delta )
{
	for ( std::vector<EventBase *>::iterator event = eventQueue.begin(); event != eventQueue.end(); ++event )
	{
		scoreKeeper.handleEvent( *event );
		switch ( ( *event )->type )
		{
		default:
			break;
		}
		delete *event;
	}
	eventQueue.clear();
}

///--- PRIVATE -----------------------------------------------------------------
