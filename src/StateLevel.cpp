#include "StateLevel.h"

#include <cmath>
#include <time.h>

#include "gameDefines.h"
#include "UtilityFunctions.h"
#include "Random.h"

#include "UnitPlayer.h"

#define LEVEL_BG_FADE_TIME 2500

StateLevel::StateLevel( const std::string &filename ) :
	StateBase(),
	scoreKeeper( this ),
	spawnHandler( this )
{
	player = new PLAYER_CLASS( this );
	*( player->x ) = APP_SCREEN_WIDTH / 2;
	*( player->y ) = APP_SCREEN_HEIGHT / 2;
//	player->props.addFlag( UnitBase::ufInvincible );

#ifdef _DEBUG
	debugText = spFontLoad( FONT_GENERAL, 12 );
	if ( debugText )
		spFontAdd( debugText, SP_FONT_GROUP_ASCII, spGetFastRGB( 255, 0, 0 ) );
#endif

	spGetInput()->button[SP_BUTTON_START] = 0;

	bgcol.r = 0;
	bgcol.g = 0;
	bgcol.b = 255;
	bgcol.intensity = 0.5;
	timers.push_back( &bgFadeTimer );
	scoreMode = ScoreNormal::smNone;

	run = new Replay();
	if ( filename[0] != 0 )
	{
		run->loadFromFile( filename );
	}
	frameCounter = 0;

	type = stLevel;
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
	delete run;
	spResetButtonsState();
	spResetAxisState();
#ifdef _DEBUG
	spFontDelete( debugText );
#endif
}


///--- PUBLIC ------------------------------------------------------------------

int StateLevel::update( Uint32 delta )
{
	RANDOM->clearCache();
	if ( run->playing )
	{
		int temp = run->playEntry();
		if ( temp < 0 )
			return stScore;
		delta = temp;
	}
	++frameCounter;

	StateBase::update( delta );

	if ( spGetInput()->button[SP_BUTTON_START] )
		return stMenu;

	delta = std::min( ( int )delta, MAX_DELTA );

#ifdef _DEBUG
	debugString = Utility::numToStr( spGetFPS() ) + " fps (" + Utility::numToStr( delta ) + ")\n";
	if ( run->playing )
		debugString += "frame: " + Utility::numToStr( frameCounter ) + " / " + Utility::numToStr( run->getFrameCount() ) + "\n";
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

	units.insert( units.end(), unitQueue.begin(), unitQueue.end() );
	unitQueue.clear();

	// Score (reads events)
	scoreKeeper.update( delta );
	if ( scoreKeeper.getMode() != scoreMode )
	{
		bgFadeTimer.start( LEVEL_BG_FADE_TIME );
		scoreMode = scoreKeeper.getMode();
		fadecol = bgcol;
	}

	// Events (reads and removes events)
	handleEvents( delta );

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

	// Replay recording
	if ( !run->playing )
		run->addEntry( delta );

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
	if ( bgFadeTimer.wasStarted() )
	{
		float timerFactor = (float)bgFadeTimer.getTime() / (float)LEVEL_BG_FADE_TIME;
		switch ( scoreMode )
		{
		case ScoreNormal::smNone:
			bgcol.r = 0 + fadecol.r * timerFactor;
			bgcol.g = 0 + fadecol.g * timerFactor;
			bgcol.b = 255 + ( fadecol.b - 255 ) * timerFactor;
			break;
		case ScoreNormal::smPeace:
			bgcol.r = 0 + fadecol.r * timerFactor;
			bgcol.g = 255 + ( fadecol.g - 255 ) * timerFactor;
			bgcol.b = 0 + fadecol.b * timerFactor;
			break;
		case ScoreNormal::smAggression:
			bgcol.r = 255 + ( fadecol.r - 255 ) * timerFactor;
			bgcol.g = 0 + fadecol.g * timerFactor;
			bgcol.b = 0 + fadecol.b * timerFactor;
			break;
		}
		if ( bgFadeTimer.isStopped() )
			bgFadeTimer.stop();
	}

	spClearTarget( spGetRGB( bgcol.r * bgcol.intensity,
							 bgcol.g * bgcol.intensity,
							 bgcol.b * bgcol.intensity ) );

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
