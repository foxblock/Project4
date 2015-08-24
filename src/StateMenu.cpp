#include "StateMenu.h"

#include "sparrowCore.h"
#include "sparrowPrimitives.h"
#include "gameDefines.h"

#define MENU_FONT_SIZE 48
#define MENU_INPUT_LAG 250

#define MENU_TEXT_SHOW_TIME 3000
#define MENU_TEXT_FADE_TIME 1000

std::size_t StateMenu::choice = 0;

StateMenu::StateMenu() : StateBase()
{
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
	addMenuEntry( "start wave game", stWave );
	addMenuEntry( "show scores", stHighscores );
	addMenuEntry( "load replay", stReplayLoader );
#ifdef _DEBUG
	addMenuEntry( "collision test", stCollision );
#endif
	addMenuEntry( "exit", -1 );
	timers.push_back( &inputLag );

	text = spCreateSurface( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT / 2 );
	textMode = 1;
	textIndex = -1;
	timers.push_back( &textTimer );
	lines.push_back( std::make_pair( "A game by", "Janek Schäfer" ) );
	lines.push_back( std::make_pair( "Press \""SP_BUTTON_B_NAME"\" or \""SP_BUTTON_START_NAME"\"", "to select an item" ) );
	lines.push_back( std::make_pair( "This is a WIP", "A lot left to do" ) );
	lines.push_back( std::make_pair( "Use the arrow keys", "to navigate" ) );
	lines.push_back( std::make_pair( "Hint: Bombs explode,", "Spikes and Lasers don't" ) );
	lines.push_back( std::make_pair( "Hint: Not entering a name,", "will not add a highscore or replay" ) );
	lines.push_back( std::make_pair( "Hint: Waves may exceed,", "the max unit limit" ) );

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
		if ( entries[choice].targetState == stWave )
			message = "waves/test.txt";
		return entries[choice].targetState;
	}

	return 0;
}

void StateMenu::render(SDL_Surface* target)
{
	spClearTarget( spGetRGB(128,0,0) );

	if ( textMode == -1 )
	{
		SDL_SetAlpha( text, SDL_SRCALPHA, (1.0f - (float)textTimer.getTime() / (float)textTimer.getDuration()) * 255.0f );
		if ( textTimer.isStopped() )
		{
			textTimer.start( MENU_TEXT_SHOW_TIME );
			textMode = 0;
			SDL_SetAlpha( text, SDL_SRCALPHA, SDL_ALPHA_OPAQUE );
		}
	}
	else if ( textMode == 0 )
	{
		if ( textTimer.isStopped() )
		{
			textTimer.start( MENU_TEXT_FADE_TIME );
			textMode = 1;
		}
	}
	else if ( textMode == 1 )
	{
		SDL_SetAlpha( text, SDL_SRCALPHA, (float)textTimer.getTime() / (float)textTimer.getDuration() * 255.0f );
		if ( textTimer.isStopped() )
		{
			++textIndex;
			if ( textIndex >= lines.size() )
				textIndex = 0;
			textTimer.start( MENU_TEXT_FADE_TIME );
			textMode = -1;
			SDL_FillRect( text, NULL, spGetRGB( 255, 0, 255 ) );
			SDL_SetColorKey( text, SDL_SRCCOLORKEY, spGetRGB( 255, 0, 255 ) );
			spSelectRenderTarget( text );
			spFontDraw( 20, 10, -1, (unsigned char*) lines[textIndex].first.c_str(), fontDark );
			spFontDraw( 20, 10 + MENU_FONT_SIZE, -1, (unsigned char*) lines[textIndex].second.c_str(), fontDark );
			spSelectRenderTarget( spGetWindowSurface() );
			SDL_SetAlpha( text, SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT );
		}
	}
	SDL_Rect rect = { 0,0,APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT / 2 };
	spUnlockRenderTarget();
	SDL_BlitSurface( text, NULL, spGetWindowSurface(), &rect );
	spLockRenderTarget();

	for ( int I = entries.size()-1; I >= 0; --I )
	{
		if ( choice == I )
		{
			spFontDrawRight( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - MENU_FONT_SIZE * (I+1), -1,
							 (unsigned char*) entries[I].name.c_str(), fontBright );
		}
		else
		{
			spFontDrawRight( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - MENU_FONT_SIZE * (I+1), -1,
							 (unsigned char*) entries[I].name.c_str(), fontDark );
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
