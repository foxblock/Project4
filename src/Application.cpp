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

bool Application::showModal( void ( *spDraw )( void ), int ( *spCalc )( Uint32 steps ) )
{
	printf( "Starting application... Version: %s\n", VERSION_STRING );
	if ( spLoop( spDraw, spCalc, 2, NULL, NULL ) != ERROR_CODE )
		return true;
	return false;
}

int Application::update( Uint32 delta )
{
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

		switch ( result )
		{
		case ERROR_CODE: // exit with error
			errorString = activeState->getLastError();
			return ERROR_CODE;
		case -1: // exit app normally
			return -1;
		case 0: // keep using current state
			break;
		case StateBase::stLevel:
			prevState = activeState;
			activeState = new StateLevel();
			break;
		case StateBase::stCollision:
			prevState = activeState;
			activeState = new StateCollision();
			break;
		case StateBase::stScore:
			prevState = activeState;
			activeState = new StateScore( (StateLevel*)prevState );
			break;
		case StateBase::stReplay:
			prevState = activeState;
			activeState = new StateLevel( prevState->message );
			break;
		case StateBase::stMenu:
			prevState = activeState;
			activeState = new StateMenu();
			break;
		case StateBase::stReplayLoader:
			prevState = activeState;
			activeState = new StateReplayLoader();
			break;
		case StateBase::stHighscores:
			prevState = activeState;
			activeState = new StateHighscores();
			break;
		default:
			printf( "%s Ignoring undefined state switch: %i\n", WARNING_STRING, result );
		}

	}

	return 0;
}

void Application::render()
{
	//spResetZBuffer();

	if ( prevState ) // Transition
	{

	}
	else if ( activeState )
	{
		activeState->render( screen );
	}

	spFlip();
}

// --- PRIVATE -----------------------------------------------------------------
