#include "StateError.h"

#include "gameDefines.h"

#define ERROR_FONT_SIZE 36

StateError::StateError( const std::string &message, const StateType &returnState )
{
	next = returnState;
	msg = message;
	font = spFontLoad( FONT_GENERAL, ERROR_FONT_SIZE );
	if ( font )
		spFontAdd( font, SP_FONT_GROUP_ASCII, spGetFastRGB( 0, 0, 0 ) );
}

StateError::~StateError()
{
	spFontDelete( font );
}


///--- PUBLIC ------------------------------------------------------------------

int StateError::update(Uint32 delta)
{
	if ( spGetInput()->button[SP_BUTTON_START] ||
		spGetInput()->button[SP_BUTTON_B] ||
		spGetInput()->button[SP_BUTTON_Y] )
	{
		spResetButtonsState();
		return next;
	}
	return 0;
}

void StateError::render(SDL_Surface* target)
{
	spClearTarget( COLOUR_BACKGROUND );
	spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 - 70, -1,
					msg.c_str(), font );
	spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT - 50, -1,
					"Press " SP_BUTTON_START_NAME " to continue.", font );
}


///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
