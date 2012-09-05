#include "StateLevel.h"

#include <cmath>
#include <time.h>
#include <utility>

#include "gameDefines.h"
#include "UtilityFunctions.h"

#include "UnitPlayer.h"

#define LEVEL_BG_FADE_TIME 2500
#define LEVEL_PAUSE_FONT_SIZE 32

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
	pauseText = spFontLoad( FONT_GENERAL, LEVEL_PAUSE_FONT_SIZE );
	if ( pauseText )
		spFontAdd( pauseText, SP_FONT_GROUP_ASCII, -1 );

	spGetInput()->button[SP_BUTTON_START] = 0;

	bgcol.r = 0;
	bgcol.g = 0;
	bgcol.b = 255;
	bgcol.intensity = 0.50;
	scoreMode = ScoreNormal::smNone;

	pauseScreen = spCreateSurface( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT );

	run = new Replay();
	if ( filename[0] != 0 )
	{
		if ( !run->loadFromFile( filename ) )
			errorString = "Could not load replay file!\n" + run->errorString;
		timecode = run->info.timecode;
	}
	else
	{
		timecode = time( NULL );
		run->info.timecode = timecode;
	}
	Utility::seedRand( timecode );

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
	spDeleteSurface( pauseScreen );
#ifdef _DEBUG
	spFontDelete( debugText );
#endif
}


///--- PUBLIC ------------------------------------------------------------------

int StateLevel::update( Uint32 delta )
{
	if ( spGetInput()->button[SP_BUTTON_START] )
	{
		SDL_Surface *temp = spCreateSurface( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT );
		spSelectRenderTarget( temp );
		render( temp );
		SDL_SetAlpha( temp, SDL_SRCALPHA, 128 );
		SDL_FillRect( pauseScreen, NULL, 0 );
		SDL_BlitSurface( temp, NULL, pauseScreen, NULL );
		spSelectRenderTarget( spGetWindowSurface() );
		spDeleteSurface( temp );
		paused = true;
		spResetAxisState();
		spResetButtonsState();
		return 0;
	}

	if ( run->playing )
	{
		if ( !run->play() )
			return stScore;
	}

	if ( errorString[0] != 0 )
	{
		return stMenu;
	}

	StateBase::update( delta );

	delta = std::min( ( int )delta, MAX_DELTA );

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

	// Background effects
	if ( scoreKeeper.getMode() != scoreMode )
	{
		//bgFadeTimer.start( LEVEL_BG_FADE_TIME );
		//fadecol = bgcol;
		scoreMode = scoreKeeper.getMode();
		ShapeCircle temp( player->shape.pos, player->shape.radius );
		colour tempCol;
		switch ( scoreMode )
		{
		case ScoreNormal::smNone:
			tempCol.r = 0;
			tempCol.g = 0;
			tempCol.b = 255;
			break;
		case ScoreNormal::smPeace:
			tempCol.r = 0;
			tempCol.g = 255;
			tempCol.b = 0;
			break;
		case ScoreNormal::smAggression:
			tempCol.r = 255;
			tempCol.g = 0;
			tempCol.b = 0;
			break;
		}
		tempCol.intensity = bgcol.intensity;
		bgEffects.push_back( std::make_pair(temp,tempCol) );
	}
	for ( std::vector< std::pair<ShapeCircle, colour> >::iterator I = bgEffects.begin(); I != bgEffects.end();  )
	{
		Vector2d<float> temp(0,0);
		if ( I->first.pos.x < APP_SCREEN_WIDTH / 2 )
			temp.x = APP_SCREEN_WIDTH;
		if ( I->first.pos.y < APP_SCREEN_HEIGHT / 2 )
			temp.y = APP_SCREEN_HEIGHT;
		temp -= I->first.pos;
		if ( temp.lengthSquared() <= Utility::sqr( I->first.radius ) )
		{
			bgcol = I->second;
			bgEffects.erase( I );
		}
		else
		{
			I->first.radius += 1;
			I++;
		}
	}

	// Events (reads and removes events)
	handleEvents( delta );

	// Replay recording
	if ( !run->playing )
		run->update( player->toBeRemoved );

	if ( player && player->toBeRemoved )
	{
		printf( "Score: %i\n", scoreKeeper.getScore() );
		return stScore;
	}

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

	return 0;
}

int StateLevel::pauseUpdate( Uint32 delta )
{
	if ( spGetInput()->button[SP_BUTTON_START] )
	{
		return stMenu;
	}
	if ( spGetInput()->button[SP_BUTTON_B] ||
		spGetInput()->button[SP_BUTTON_Y] ||
		spGetInput()->axis[0] != 0 ||
		spGetInput()->axis[1] != 0 )
	{
		paused = false;
	}
	return 0;
}

void StateLevel::render( SDL_Surface *target )
{
	if ( errorString[0] != 0 )
		return;

	spClearTarget( spGetRGB( bgcol.r * bgcol.intensity,
							 bgcol.g * bgcol.intensity,
							 bgcol.b * bgcol.intensity ) );

	for ( std::vector< std::pair< ShapeCircle, colour> >::const_iterator I = bgEffects.begin(); I != bgEffects.end(); ++I )
	{
		spEllipse( I->first.pos.x, I->first.pos.y, -1, I->first.radius, I->first.radius,
					spGetRGB( I->second.r * I->second.intensity,
							I->second.g * I->second.intensity,
							I->second.b * I->second.intensity ) );
	}

	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		( *I )->render( target );

	if ( player )
		player->render( target );

	scoreKeeper.render( target );

#ifdef _DEBUG
	debugString = Utility::numToStr( spGetFPS() ) + " fps\n";
	if ( run->playing )
		debugString += "frame: " + Utility::numToStr( run->frameCounter ) + " / " + Utility::numToStr( run->totalFrames ) + "\n";

	debugString += Utility::numToStr( units.size() ) + " units\n";
	if ( debugText )
		spFontDraw( 5, 5, -1, debugString.c_str(), debugText );
	spawnHandler.render( target );
#endif
}

void StateLevel::pauseRender( SDL_Surface *target )
{
	SDL_BlitSurface( pauseScreen, NULL, spGetWindowSurface(), NULL );
	spFontDrawMiddle( APP_SCREEN_WIDTH / 2, 200, -1, "Game paused.", pauseText );
	spFontDrawMiddle( APP_SCREEN_WIDTH / 2, 200 + LEVEL_PAUSE_FONT_SIZE, -1,
					"Press any key to resume or \""SP_BUTTON_START_NAME"\" to exit.", pauseText );
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
