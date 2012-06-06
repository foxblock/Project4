#include "Application.h"

#include "StateLevel.h"

Application::Application()
{
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
	if ( spLoop( spDraw, spCalc, 0, NULL, NULL ) != ERROR_CODE )
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
		case 1: // StateLevel
			prevState = activeState;
			activeState = new StateLevel();
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
	spClearTarget( SDL_MapRGB( screen->format, 0, 200, 200 ) );

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
