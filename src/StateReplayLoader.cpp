#include "StateReplayLoader.h"

#include "sparrowCore.h"
#include "sparrowPrimitives.h"
#include "gameDefines.h"

#define REPLAY_FONT_SIZE 32
#define REPLAY_CARET_BLINK_TIME 600

StateReplayLoader::StateReplayLoader()
{
	filenameBuffer[0] = '\0';
	font = spFontLoad( FONT_GENERAL, REPLAY_FONT_SIZE );
	if ( font )
	{
		spFontAdd( font, SP_FONT_GROUP_ALPHABET SP_FONT_GROUP_GERMAN SP_FONT_GROUP_NUMBERS ".:!\"_", 0 );
	}
	spPollKeyboardInput( filenameBuffer, 255, 1 );
	caretTimer.start( REPLAY_CARET_BLINK_TIME );
	timers.push_back( &caretTimer );
	caret = true;

	type = stReplayLoader;
}

StateReplayLoader::~StateReplayLoader()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

int StateReplayLoader::update(Uint32 delta)
{
	StateBase::update( delta );

	if ( caretTimer.isStopped() )
	{
		caret = !caret;
		caretTimer.start( REPLAY_CARET_BLINK_TIME );
	}

	if ( spGetInput()->button[SP_BUTTON_START] )
	{
		spResetButtonsState();
		spStopKeyboardInput( );
		message = FOLDER_REPLAY "/";
		message += filenameBuffer;
		message += EXTENSION_REPLAY;
		return stReplay;
	}

	return 0;
}

void StateReplayLoader::render(SDL_Surface* target)
{
	spClearTarget( COLOUR_BACKGROUND );
	if ( font )
	{
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 - REPLAY_FONT_SIZE, -1, "Enter the name of the replay file to load:", font );
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 , -1, "(no file extension, assuming position in replay folder)", font );
		char temp[strlen(filenameBuffer) + caret];
		strcpy( temp, filenameBuffer );
		if ( caret )
			strcat( temp, "_\0" );
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 + REPLAY_FONT_SIZE, -1, temp, font);
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 + REPLAY_FONT_SIZE * 3, -1, "Press \""SP_BUTTON_START_NAME"\" to load...", font );
	}
	if (spIsInputLocked())
		spBlitSurface(APP_SCREEN_WIDTH/2,APP_SCREEN_HEIGHT-spGetVirtualKeyboard()->h/2,0,spGetVirtualKeyboard());
}


///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
