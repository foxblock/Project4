#include "StateHighscores.h"

#include "sparrowCore.h"
#include "sparrowPrimitives.h"
#include "UtilityFunctions.h"
#include "gameDefines.h"

#define HIGHS_FONT_SIZE 36
#define HIGHS_INPUT_LAG 20

const int HIGHS_ENTRIES_ON_SCREEN = ( APP_SCREEN_HEIGHT - 20 ) / HIGHS_FONT_SIZE;

StateHighscores::StateHighscores() :
	StateBase(),
	file( FOLDER_DATA "/" FILE_HIGHSCORE_NORMAL )
{
	font = spFontLoad( FONT_GENERAL, HIGHS_FONT_SIZE );
	if ( font )
	{
		spFontAdd( font, SP_FONT_GROUP_ALPHABET SP_FONT_GROUP_GERMAN ".:!\"_" SP_FONT_GROUP_NUMBERS, -1 );
	}
	offset = 0;
	offsetIter = file.scores.begin();
	timers.push_back( &inputLag );

	type = stHighscores;
}

StateHighscores::~StateHighscores()
{
	spFontDelete( font );
	spResetAxisState();
}


///--- PUBLIC ------------------------------------------------------------------

int StateHighscores::update(Uint32 delta)
{
	StateBase::update( delta );

	if ( inputLag.isStopped() )
	{
		if ( spGetInput()->axis[1] < 0 && offset > 0 )
		{
			--offset;
			--offsetIter;
		}
		else if ( spGetInput()->axis[1] > 0 && offset < file.scores.size() - HIGHS_ENTRIES_ON_SCREEN )
		{
			++offset;
			++offsetIter;
		}
		inputLag.start( HIGHS_INPUT_LAG );
	}

	if ( spGetInput()->button[ SP_BUTTON_START ] )
	{
		spResetButtonsState();
		return stMenu;
	}

	return 0;
}

void StateHighscores::render(SDL_Surface* target)
{
	spClearTarget( COLOUR_BACKGROUND );

	int posOffset = spFontWidth( Utility::numToStr( file.scores.size() ).c_str(), font );
	int numEntries = std::min( HIGHS_ENTRIES_ON_SCREEN, (int)file.scores.size() );
	int vertOffset = ( APP_SCREEN_HEIGHT - numEntries * HIGHS_FONT_SIZE ) / 2;

	int I = offset;
	for ( HighscoreFile::scoreIter iter = offsetIter; I < offset + numEntries && iter != file.scores.end(); ++iter )
	{
		int yPos = APP_SCREEN_HEIGHT - vertOffset - ( I - offset + 1 ) * HIGHS_FONT_SIZE;
		spFontDrawRight( posOffset + 14, yPos, -1,
						 ( Utility::numToStr( I + 1 ) + "." ).c_str(), font );
		spFontDraw( posOffset + 25, yPos, -1,
					iter->name.c_str(), font );
		spFontDrawRight( APP_SCREEN_WIDTH - 16, yPos, -1,
						Utility::numToStr( iter->score ).c_str(), font );
		++I;
	}
}


///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
