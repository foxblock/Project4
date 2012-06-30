#include "Application.h"

#include "sparrow3d.h"
#include <time.h>

#include "gameDefines.h"

#include "StateLevel.h"
#include "StateCollision.h"
#include "StateScore.h"

#ifdef _DEBUG
#define STARTING_STATE StateCollision
#else
#define STARTING_STATE StateLevel
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

	srand( time( NULL ) );
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
		int result = activeState->update( delta );

		switch ( result )
		{
		case ERROR_CODE: // exit with error
			errorString = activeState->getLastError();
			return ERROR_CODE;
		case -1: // exit app normally
			return -1;
		case 0: // keep using current state
			break;
		case StateBase::stLevel: // StateLevel
			prevState = activeState;
			activeState = new StateLevel();
			break;
		case StateBase::stCollision: // StateCollision
			prevState = activeState;
			activeState = new StateCollision();
			break;
		case StateBase::stScore:
			prevState = activeState;
			activeState = new StateScore( (StateLevel*)prevState );
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
	else
	{
		activeState->render( screen );
	}

	spFlip();
}

// --- PRIVATE -----------------------------------------------------------------
