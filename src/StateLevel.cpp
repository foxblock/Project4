#include "StateLevel.h"

#include <cmath>
#include <time.h>
#include <utility>

#include "gameDefines.h"

#include "UnitPlayer.h"
#include "UnitText.h"
#include "ScoreNormal.h"
#include "SpawnNormal.h"

#include "sparrowPrimitives.h"

#define LEVEL_BG_FADE_TIME 2500
#define LEVEL_PAUSE_FONT_SIZE 32

#define LEVEL_SLOWMO_LEVEL 1

StateLevel::StateLevel( Replay *loadReplay ) :
	StateBase(),
	__scoreKeeper( this ),
	__spawnHandler( this )
{
	spawnHandler = &__spawnHandler;
	scoreKeeper = &__scoreKeeper;
	player = new UnitPlayer( this );
	*( player->x ) = APP_SCREEN_WIDTH / 2;
	*( player->y ) = APP_SCREEN_HEIGHT / 2;
	addUnit( player, false );
//	player->flags.add( UnitBase::ufInvincible );

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
	bgcol.intensity = 0.7;

	pauseScreen = spCreateSurface( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT );

	if ( loadReplay )
	{
		run = loadReplay;
		timecode = run->info.timecode;
	}
	else
	{
		run = new Replay();
		timecode = time( NULL );
		run->info.timecode = timecode;
		run->info.levelType = stLevel;
		run->info.parameter = "";
	}
	Utility::seedRand( timecode );

	slowmo = false;
	slowmoCounter = LEVEL_SLOWMO_LEVEL;
	timers.push_back( &slowmoTimer );

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
		spUnlockRenderTarget();
		SDL_SetAlpha( temp, SDL_SRCALPHA, 128 );
		SDL_FillRect( pauseScreen, NULL, 0 );
		SDL_BlitSurface( temp, NULL, pauseScreen, NULL );
		spLockRenderTarget();
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

	delta = std::min( ( int )delta, MAX_DELTA );

	deltaBkUp = delta;
	if ( slowmo )
	{
		if ( slowmoCounter > 0 || ( Utility::sign( LEVEL_SLOWMO_LEVEL ) < 0 && slowmoCounter == 0 ) )
		{
			delta = 0;
		}
		if ( slowmoCounter == 0 )
			slowmoCounter = LEVEL_SLOWMO_LEVEL;
		else
			slowmoCounter -= Utility::sign( slowmoCounter );
	}
	if ( slowmoTimer.stopped() && slowmo )
		slowmo = false;

	StateBase::update( delta );

	LOG_MESSAGE("Unit update and checks");

	// Unit update, collision checking (creates events)
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
	{
		if ( (*I)->flags.has( UnitBase::ufFrozen ) )
			continue;

		if ( player )
			( *I )->ai( delta, player );
		( *I )->update( (*I)->flags.has( UnitBase::ufIsPlayer ) ? deltaBkUp : delta );
		for ( std::vector<UnitBase *>::iterator K = I + 1; K != units.end(); ++K )
		{
			if ( ( *I )->checkCollision( *K ) )
			{
				( *I )->collisionResponse( *K );
				( *K )->collisionResponse( *I );
			}
		}
	}

	LOG_MESSAGE("Spawn checks");

	// Spawning (creates events)
	spawnHandler->update( delta );

	units.insert( units.end(), unitQueue.begin(), unitQueue.end() );
	unitQueue.clear();

	LOG_MESSAGE("Score stuff");

	// Score (reads events)
	scoreKeeper->update( delta );

	LOG_MESSAGE("Effects");

	// Background effects
	for ( std::vector< std::pair<ShapeCircle, Utility::colour> >::iterator I = bgEffects.begin(); I != bgEffects.end(); )
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

	LOG_MESSAGE("Event checks");

	// Events (reads and removes events)
	handleEvents( delta );

	// Replay recording
	if ( !run->playing )
		run->update( player->toBeRemoved );

	if ( player && player->toBeRemoved )
	{
		printf( "YOU DIED! Score: %i\n", scoreKeeper->getScore() );
		return stScore;
	}

	LOG_MESSAGE("Final unit handling");

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

	for ( std::vector< std::pair< ShapeCircle, Utility::colour> >::const_iterator I = bgEffects.begin(); I != bgEffects.end(); ++I )
	{
		spEllipse( I->first.pos.x, I->first.pos.y, -1, I->first.radius, I->first.radius,
					spGetRGB( I->second.r * I->second.intensity,
							I->second.g * I->second.intensity,
							I->second.b * I->second.intensity ) );
	}

	// render text
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		if ( ( *I )->flags.has( UnitBase::ufBackground ) )
			( *I )->render( target );
	for ( std::vector<UnitBase *>::iterator I = units.begin(); I != units.end(); ++I )
		if ( !( *I )->flags.has( UnitBase::ufBackground ) )
			( *I )->render( target );

	if ( player )
		player->render( target );

	scoreKeeper->render( target );

#ifdef _DEBUG
	debugString = Utility::numToStr( spGetFPS() ) + " fps (" + Utility::numToStr<int>( deltaBkUp ) + ")\n";
	if ( run->playing )
		debugString += "frame: " + Utility::numToStr( run->frameCounter ) + " / " + Utility::numToStr( run->totalFrames ) + "\n";

	debugString += Utility::numToStr( units.size() ) + " units\n";
	if ( debugText )
		spFontDraw( 5, 5, -1, (unsigned char*) debugString.c_str(), debugText );
#endif
	spawnHandler->render( target );
}

void StateLevel::pauseRender( SDL_Surface *target )
{
	spUnlockRenderTarget();
	SDL_BlitSurface( pauseScreen, NULL, spGetWindowSurface(), NULL );
	spLockRenderTarget();
	spFontDrawMiddle( APP_SCREEN_WIDTH / 2, 200, -1, (unsigned char*) "Game paused.", pauseText );
	spFontDrawMiddle( APP_SCREEN_WIDTH / 2, 200 + LEVEL_PAUSE_FONT_SIZE, -1,
					(unsigned char*) "Press any key to resume or \""SP_BUTTON_START_NAME"\" to exit.", pauseText );
}

void StateLevel::addUnit( UnitBase *newUnit, const bool &generateEvent )
{
#ifdef _DEBUG
	printf("%s:%i\t\t%s, type: %i\n",__FILE__,__LINE__,"New unit is being added",newUnit->type);
#endif
	if ( generateEvent )
	{
		EventUnitSpawn *event = new EventUnitSpawn( newUnit );
		addEvent( event );
	}
	unitQueue.push_back( newUnit );
}

void StateLevel::addEvent( EventBase *newEvent )
{
#ifdef _DEBUG
	printf("%s:%i\t\t%s, type: %i\n",__FILE__,__LINE__,"New event is being added",newEvent->type);
#endif
	eventQueue.push_back( newEvent );
}

///--- PROTECTED ---------------------------------------------------------------

void StateLevel::handleEvents( Uint32 delta )
{
	std::vector<EventBase *> eventList = eventQueue;
	eventQueue.clear();

	for ( std::vector<EventBase *>::iterator event = eventList.begin(); event != eventList.end(); ++event )
	{
		scoreKeeper->handleEvent( *event );
		spawnHandler->handleEvent( *event );
		switch ( ( *event )->type )
		{
		case EventBase::etSlowMotion:
			slowmo = true;
			slowmoTimer.start( ((EventSlowMotion*)*event)->duration );
			break;
		case EventBase::etScoreMode:
		{
			ShapeCircle temp( player->shape.pos, player->shape.radius );
			Utility::colour tempCol;
			switch ( ((EventScoreModeChange*)*event)->newMode )
			{
			case ScoreNormal::smNone:
				tempCol.r = 0;
				tempCol.g = 0;
				tempCol.b = 255;
				tempCol.intensity = 0.7;
				break;
			case ScoreNormal::smPeace:
				tempCol.r = 0;
				tempCol.g = 255;
				tempCol.b = 0;
				tempCol.intensity = 0.5;
				break;
			case ScoreNormal::smAggression:
				tempCol.r = 255;
				tempCol.g = 0;
				tempCol.b = 0;
				tempCol.intensity = 0.5;
				break;
			}
			//tempCol.intensity = bgcol.intensity;
			bgEffects.push_back( std::make_pair(temp,tempCol) );
			break;
		}
		case EventBase::etUnitDeath:
		{
			EventUnitDeath *temp = (EventUnitDeath*)*event;
			if ( temp->points < 0 )
				break;
			UnitText *text = new UnitText( this );
			*(text->x) = *(temp->unit->x);
			*(text->y) = *(temp->unit->y);
			text->alignment = UnitText::taCentre;
			text->fontSize = 24;
			text->colour1 = -1;
			text->alpha1 = 128;
			text->text = "kill +" + Utility::numToStr( temp->points );
			text->life = 500;
			text->mode = UnitText::tmStatic;
			addUnit( text, false );
			break;
		}
		case EventBase::etWaveSkip:
		{
			UnitText *text = new UnitText( this );
			*(text->x) = 400;
			*(text->y) = 240;
			text->alignment = UnitText::taCentre;
			text->fontSize = 24;
			text->colour1 = -1;
			text->alpha1 = 128;
			text->text = "wave skip +" + Utility::numToStr( ((EventWaveSkip*)*event)->timeAmount );
			text->life = 1000;
			text->mode = UnitText::tmStatic;
			addUnit( text, false );
			break;
		}
		default:
			break;
		}
		delete *event;
	}
	eventList.clear();
}

///--- PRIVATE -----------------------------------------------------------------
