#include "Application.h"

#include "sparrow3d.h"

#include "gameDefines.h"

#include "StateLevel.h"
#include "StateCollision.h"
#include "StateScore.h"
#include "StateMenu.h"
#include "StateReplayLoader.h"
#include "StateHighscores.h"
#include "StateError.h"
#include "StateWave.h"
#include "UtilityFunctions.h"
#include "Replay.h"

#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#ifdef _DEBUG
#define STARTING_STATE StateMenu
#else
#define STARTING_STATE StateMenu
#endif

Application::Application()
{
	SDL_putenv("SDL_VIDEO_WINDOW_POS=500,200");

	spInitCore();

	screen = spCreateWindow( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT, APP_FULLSCREEN, APP_RESIZE );
	spSelectRenderTarget( spGetWindowSurface() );
	spSetHorizontalOrigin( SP_CENTER );
	spSetVerticalOrigin( SP_CENTER );
	spSetZSet( 0 );
	spSetZTest( 0 );

	errorString = "";
	activeState = new STARTING_STATE();
	prevState = NULL;

	// Create important folders
	#ifdef WIN32
	mkdir( FOLDER_REPLAY );
	mkdir( FOLDER_DATA );
	#else
	mkdir( FOLDER_REPLAY, 0xFFFF );
	mkdir( FOLDER_DATA, 0xFFFF );
	#endif
}

Application::~Application()
{
	delete activeState;
	delete prevState;

	spQuitCore();
}

// --- PUBLIC ------------------------------------------------------------------

StateBase* Application::getState( const int &type, StateBase const * const passingState )
{
	StateBase *result = NULL;
	switch ( type )
	{
	case StateBase::stLevel:
		result = new StateLevel();
		break;
	case StateBase::stCollision:
		result = new StateCollision();
		break;
	case StateBase::stScore:
		if ( !passingState )
		{
			errorString = "Needs passingState to construct state type: " + Utility::numToStr( type );
			return NULL;
		}
		result = new StateScore( (StateLevel*)passingState );
		break;
	case StateBase::stReplay:
	{
		if ( !passingState )
		{
			errorString = "Needs passingState to construct state type: " + Utility::numToStr( type );
			return NULL;
		}
		Replay *run = new Replay();
		if ( !run->loadFromFile( passingState->message ) )
		{
			errorString = "Could not load replay file!\n" + run->errorString;
			break;
		}
		switch ( run->info.levelType )
		{
		case StateBase::stLevel:
			result = new StateLevel( run );
			break;
		case StateBase::stWave:
			result = new StateWave( run->info.parameter, run );
			break;
		default:
			errorString = "Malformed Replay header, unknown level type: " +
					Utility::numToStr( run->info.levelType );
		}
		break;
	}
	case StateBase::stMenu:
		result = new StateMenu();
		break;
	case StateBase::stReplayLoader:
		result = new StateReplayLoader();
		break;
	case StateBase::stHighscores:
		result = new StateHighscores();
		break;
	case StateBase::stWave:
		if ( !passingState )
		{
			errorString = "Needs passingState to construct state type: " + Utility::numToStr( type );
			return NULL;
		}
		result = new StateWave( passingState->message, NULL );
		break;
	default:
		printf( "%s Ignoring undefined state switch: %i\n", WARNING_STRING, type );
	}

	return result;
}

bool Application::showModal( void ( *spDraw )( void ), int ( *spCalc )( Uint32 steps ) )
{
	printf( "Starting application... Version: %s\n", VERSION_STRING );

	Utility::seedRand( 987654321 );
	printf("Random test: %i %i %i %i\n",Utility::randomRange(-100,100),Utility::randomRange(-100,100),Utility::randomRange(-100,100),Utility::randomRange(-100,100));
	printf("Control group: %i %i %i %i\n",-3, -4, 37, -81);

	if ( spLoop( spDraw, spCalc, 2, NULL, NULL ) != ERROR_CODE )
		return true;
	return false;
}

int Application::update( Uint32 delta )
{
	LOG_MESSAGE("Running state update...");

	if ( prevState ) // Transition
	{
		delete prevState;
		prevState = NULL;
	}
	else
	{
		int result = 0;
		for ( int I = 0; I < delta; ++I )
		{
			if ( activeState->paused )
				result = activeState->pauseUpdate( 1 );
			else
				result = activeState->update( 1 );
			if ( result != 0 )
				break;
		}

		if ( activeState->getLastError()[0] != 0 )
		{
			printf( "State %i returned error: %s\n", activeState->type, activeState->getLastError().c_str() );
			prevState = activeState;
			activeState = new StateError( prevState->getLastError(), (StateBase::StateType)result );
			return 0;
		}

		StateBase *temp = prevState;
		prevState = activeState;
		activeState = NULL;
		switch ( result )
		{
		case ERROR_CODE: // exit with error
			errorString = prevState->getLastError();
			return ERROR_CODE;
		case -1: // exit app normally
			return -1;
		case 0: // keep using current state
			break;
		default:
			activeState = getState( result, prevState );
		}
		if ( !activeState )
		{
			if ( errorString[0] == 0 )
			{
				activeState = prevState;
				prevState = temp;
			}
			else
			{
				printf( "Error during state switch: %s\n", errorString.c_str() );
				activeState = new StateError( errorString, StateBase::stMenu );
				errorString = "";
			}
		}

	}

	return 0;
}

void Application::render()
{
	LOG_MESSAGE("Rendering...");

	if ( prevState ) // Transition
	{

	}
	else if ( activeState )
	{
		if ( activeState->paused )
			activeState->pauseRender( screen );
		else
			activeState->render( screen );
	}

	spFlip();
}

// --- PRIVATE -----------------------------------------------------------------
