#include "StateMenu.h"

#include "sparrowCore.h"
#include "sparrowPrimitives.h"
#include "gameDefines.h"

#define MENU_FONT_SIZE 48
#define MENU_INPUT_LAG 250

StateMenu::StateMenu() : StateBase()
{
	choice = 0;
	fontDark = spFontLoad( FONT_GENERAL, MENU_FONT_SIZE );
	if ( fontDark )
	{
		spFontAdd( fontDark, SP_FONT_GROUP_ASCII, spGetFastRGB( 0, 0, 0 ) );
		spFontAdd( fontDark, SP_FONT_GROUP_GERMAN, spGetFastRGB( 0, 0, 0 ) );
	}
	fontBright = spFontLoad( FONT_GENERAL, MENU_FONT_SIZE );
	if ( fontBright )
		spFontAdd( fontBright, SP_FONT_GROUP_ASCII, spGetFastRGB( 255, 255, 255 ) );

	addMenuEntry( "start game", stLevel );
	addMenuEntry( "show scores", stHighscores );
	addMenuEntry( "load replay", stReplayLoader );
#ifdef _DEBUG
	addMenuEntry( "collision test", stCollision );
#endif
	addMenuEntry( "exit", -1 );
	timers.push_back( &inputLag );

	type = stMenu;
}

StateMenu::~StateMenu()
{
	spFontDelete( fontDark );
	spFontDelete( fontBright );
	spResetAxisState();
	spResetButtonsState();
}


///--- PUBLIC ------------------------------------------------------------------

int StateMenu::update(Uint32 delta)
{
	StateBase::update( delta );

	if ( inputLag.isStopped() )
	{
		if ( spGetInput()->axis[1] > 0 )
			choice = std::min( --choice, entries.size()-1 );
		else if ( spGetInput()->axis[1] < 0 )
		{
			++choice;
			if ( choice > entries.size()-1 )
				choice = 0;
		}
		inputLag.start( MENU_INPUT_LAG );
	}
	if ( spGetInput()->axis[1] == 0 )
	{
		inputLag.stop();
	}

	if ( spGetInput()->button[SP_BUTTON_B] ||
		 spGetInput()->button[SP_BUTTON_Y] ||
		 spGetInput()->button[SP_BUTTON_START] )
	{
		spResetButtonsState();
		return entries[choice].targetState;
	}

	return 0;
}

void StateMenu::render(SDL_Surface* target)
{
	spClearTarget( spGetRGB(128,0,0) );

	spFontDraw( 20, 10, -1, "A game by", fontDark );
	spFontDraw( 20, 10 + MENU_FONT_SIZE, -1, "Janek Schäfer", fontDark );

	for ( int I = entries.size()-1; I >= 0; --I )
	{
		if ( choice == I )
		{
			spFontDrawRight( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - MENU_FONT_SIZE * (I+1), -1,
							 entries[I].name.c_str(), fontBright );
		}
		else
		{
			spFontDrawRight( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - MENU_FONT_SIZE * (I+1), -1,
							 entries[I].name.c_str(), fontDark );
		}
	}
}

///--- PROTECTED ---------------------------------------------------------------

void StateMenu::addMenuEntry( const std::string &name, const int &targetState )
{
	MenuEntry entry = { name, targetState };
	entries.push_back( entry );
}

///--- PRIVATE -----------------------------------------------------------------
